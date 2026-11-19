//
//  apeDensity.cpp — granulatore di un file audio (vedi apeDensity.h)
//

#include "apeDensity.h"
#include <cstring>

apeDensity::apeDensity (int maxOverlap, double sr)
:   apeGranulator (maxOverlap, sr), _Frequency (1.0), _RndFrequency (0.0), _Phase (0.0), _dir (1.0), _waveSeconds (0.0)
{
    setDefaults();
}

void apeDensity::setDefaults()
{
    setLevel (0.5);
    setWidth (0.0);
    setPanningMode (0);
    setLength (85.0);
    setDensity (10.0);
    setRndLength (0.0);
    setRndDensity (0.0);
    setPitch (0.0);
    setRndPitch (0.0);
    setPhase (0.0);
}

void apeDensity::setPitch (double semitones)     { if (semitones == semitones) _Frequency = pow (2.0, semitones / 12.0); }
void apeDensity::setRndPitch (double semitones)  { if (semitones == semitones) _RndFrequency = semitones / 24.0; }
void apeDensity::setInvert (int mode)            { _dir = (mode == 1 ? -1.0 : 1.0); }

void apeDensity::setPhase (double ph)
{
    if (! (ph == ph)) return;
    _Phase = ph < 0.0 ? 0.0 : (ph > 1.0 ? 1.0 : ph);
}

// [2026] usa la durata vera del file (i suoi campioni e la SUA frequenza di campionamento)
void apeDensity::setSamplePosition (double ms)
{
    const double seconds = _waveSeconds.load (std::memory_order_relaxed);
    if (seconds > 0.0)
        setPhase ((ms * 0.001) / seconds);
}

bool apeDensity::prepareGrain (apeGrain& g)
{
    const double rnd = bipolar (_RndFrequency.load (std::memory_order_relaxed));   // estratto comunque, come nel 2022

    if (! _wave.valid() || ! _win.valid())
        return false;

    // frequenza "naturale": un giro di fase = tutto il file alla sua velocita'.
    // [2026] conta la frequenza di campionamento del file, non quella di Max.
    const double freqNat   = _wave.sr / (double) _wave.frames;
    const double frequency = (_Frequency.load (std::memory_order_relaxed) * freqNat) * pow (2.0, rnd);

    g.incr = frequency / getAbsoluteSR();
    g.phi  = _Phase.load (std::memory_order_relaxed);
    return true;
}

bool apeDensity::renderGrain (apeGrain& g, double* L, double* R, long n)
{
    return g.render (L, R, n, _wave, _win, interp(), _dir.load (std::memory_order_relaxed));
}

void apeDensity::Processing (double* outL, double* outR, long sampleframes, const apeTable& wave, const apeTable& win)
{
    if (! wave.valid() || ! win.valid())
    {
        // senza tabelle non si puo' suonare: silenzio (i grani in corso restano fermi)
        memset (outL, 0, sizeof (double) * (size_t) sampleframes);
        memset (outR, 0, sizeof (double) * (size_t) sampleframes);
        return;
    }

    _wave = wave;
    _win  = win;
    _waveSeconds.store (wave.sr > 0.0 ? (double) wave.frames / wave.sr : 0.0, std::memory_order_relaxed);

    run (outL, outR, sampleframes);
}
