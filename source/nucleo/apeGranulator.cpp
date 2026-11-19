//
//  apeGranulator.cpp — la parte comune ai due granulatori (vedi apeGranulator.h)
//

#include "apeGranulator.h"
#include <cstring>
#include <ctime>
#include <cstdint>

static inline double limit (double x, double lo, double hi) { return x < lo ? lo : (x > hi ? hi : x); }
static inline double positive (double x) { return x == x ? std::fabs (x) : 0.0; }   // senza segno, e senza valori non validi

apeGranulator::apeGranulator (int maxVoices, double sr)
:   _SR (sr > 0.0 ? sr : 44100.0), _Volume (0.5), _Width (0.0), _Length (0.085), _Density (10.0),
    _RndLength (0.0), _RndDensity (0.0), _lastLength (0.085),
    _Start (false), _PanningMode (false), _interp (false),
    _overlap (maxVoices > 0 ? maxVoices : 1), _grainRequests (0), _numActivePublic (0), _newSeed (0),
    _seedChanged (false)
{
    const int n = maxVoices > 0 ? maxVoices : 1;
    _voices.resize ((size_t) n);
    _active.resize ((size_t) n);
    _free.resize ((size_t) n);
    for (int i = 0; i < n; ++i) _free[(size_t) i] = n - 1 - i;     // la voce 0 sara' la prima usata
    _numFree = n;
    _seedRnd = (int) time (nullptr);
}

void apeGranulator::setToggle (int toggle)       { _Start = toggle > 0; }
void apeGranulator::setLevel (double v)          { _Volume = (v == v ? v : 0.0); }
void apeGranulator::setWidth (double w)          { _Width = limit (positive (w), 0.0, 1.0); }
void apeGranulator::setPanningMode (int mode)    { _PanningMode = mode > 0; }
void apeGranulator::setLength (double ms)        { _Length = positive (ms) * 0.001; }
void apeGranulator::setDensity (double gps)      { const double d = positive (gps); if (d > 0.0) _Density = d; }
// [2026] limite 0 - 1: oltre, durata e densita' potevano diventare negative
void apeGranulator::setRndLength (double a)      { _RndLength  = limit (positive (a), 0.0, 1.0) * 0.5; }
void apeGranulator::setRndDensity (double a)     { _RndDensity = limit (positive (a), 0.0, 1.0) * 0.5; }
void apeGranulator::setInterp (int i)            { _interp = i > 0; }
void apeGranulator::setAbsoluteSR (double sr)    { if (sr > 0.0) _SR = sr; }
void apeGranulator::setSeed (int seed)           { _newSeed = seed; _seedChanged = true; }

// [2026] le voci sono create una volta sola: "overlap" cambia solo un numero.
// Prima cancellava e ricreava i grani mentre il thread audio li usava.
void apeGranulator::setOverlap (int maxOverlap)
{
    if (maxOverlap > 0)
        _overlap = maxOverlap < getMaxVoices() ? maxOverlap : getMaxVoices();
}

// [2026] il grano "a mano" lo fa partire il thread audio, con i parametri del momento
void apeGranulator::grain()                      { _grainRequests.fetch_add (1, std::memory_order_relaxed); }

// Generatore del 2022 (moltiplicazione per 16807), scritto senza uscire dai limiti di "int"
double apeGranulator::unipolar (double amp)
{
    _seedRnd = (int) ((uint32_t) _seedRnd * 16807u);
    return ((_seedRnd * 4.6566129e-010 + 1) * .5) * amp;
}

double apeGranulator::bipolar (double amp)
{
    _seedRnd = (int) ((uint32_t) _seedRnd * 16807u);
    return ((_seedRnd * 4.6566129e-010 + 1) - 1.0) * amp;
}

void apeGranulator::newGrain (int delay)
{
    const double sr      = _SR.load (std::memory_order_relaxed);
    const double width   = _Width.load (std::memory_order_relaxed);
    const double length  = _Length.load (std::memory_order_relaxed);
    const double density = _Density.load (std::memory_order_relaxed);

    // i numeri casuali sono estratti nello stesso ordine del 2022
    const double rndWidth = unipolar (width);
    const double grainLen = length + bipolar (_RndLength.load (std::memory_order_relaxed) * length);
    _lastLength.store (grainLen, std::memory_order_relaxed);

    double gps = density + bipolar (_RndDensity.load (std::memory_order_relaxed) * density);
    if (! (gps > 0.001)) gps = 0.001;
    const double period = sr / gps;
    // limite 4 miliardi: su Windows "unsigned long" e' a 32 bit
    _thres = period < 1.0 ? 1ul : (period > 4.0e9 ? 4000000000ul : (unsigned long) period);

    apeGrain g;
    if (! prepareGrain (g))                              // frequenza, fase, glissando... (classe derivata)
        return;

    const double lenSamples = grainLen * sr;
    if (! (lenSamples >= 1.0))                           // grano piu' corto di un campione: niente
        return;

    g.amp     = _Volume.load (std::memory_order_relaxed);
    g.envIncr = 1.0 / lenSamples;
    g.envPhi  = 0.0;
    g.delay   = delay;

    if (_PanningMode.load (std::memory_order_relaxed)) { g.left = 1.0 - width;            g.right = width; }
    else                                               { g.left = rndWidth + (1.0 - width); g.right = 1.0 - rndWidth; }

    if (_numFree == 0 || _numActive >= _overlap.load (std::memory_order_relaxed))
        return;                                          // tutte le voci permesse sono occupate

    const int v = _free[(size_t) --_numFree];
    _voices[(size_t) v] = g;
    _active[(size_t) _numActive++] = v;
}

void apeGranulator::run (double* L, double* R, long n)
{
    memset (L, 0, sizeof (double) * (size_t) n);
    memset (R, 0, sizeof (double) * (size_t) n);

    if (_seedChanged.exchange (false))
        _seedRnd = _newSeed.load();

    // grani chiesti con il messaggio "grain" (al massimo 16 per blocco)
    int requests = _grainRequests.exchange (0, std::memory_order_relaxed);
    if (requests > 16) requests = 16;
    for (int i = 0; i < requests; ++i)
        newGrain (0);

    // metronomo. Nel 2022 era un ciclo su tutti i campioni; qui si salta direttamente
    // al campione in cui parte il prossimo grano. Il risultato e' lo stesso.
    const bool start = _Start.load (std::memory_order_relaxed);
    long j = 0;
    while (j < n)
    {
        const unsigned long left = (unsigned long) (n - j);
        if (! start) { _count += left; break; }

        const unsigned long wait = _thres > _count + 1 ? _thres - _count - 1 : 0;
        if (wait >= left) { _count += left; break; }

        j += (long) wait;
        newGrain ((int) j);
        _count = 0;
        ++j;
    }

    // le voci: solo quelle che suonano
    for (int i = 0; i < _numActive; )
    {
        const int v = _active[(size_t) i];
        apeGrain& g = _voices[(size_t) v];
        const int d = g.delay;
        g.delay = 0;

        if (renderGrain (g, L + d, R + d, n - d))
            ++i;
        else
        {
            _active[(size_t) i] = _active[(size_t) --_numActive];   // tolta dall'elenco...
            _free[(size_t) _numFree++] = v;                         // ...e di nuovo libera
        }
    }

    _numActivePublic.store (_numActive, std::memory_order_relaxed);
}
