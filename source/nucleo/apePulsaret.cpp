//
//  apePulsaret.cpp — sintesi pulsar con due forme d'onda (vedi apePulsaret.h)
//

#include "apePulsaret.h"
#include <cstring>

static inline double unit (double x) { x = (x == x ? std::fabs (x) : 0.0); return x > 1.0 ? 1.0 : x; }

apePulsaret::apePulsaret (int maxOverlap, double sr)
:   apeGranulator (maxOverlap, sr), _Frequency (440.0), _RndFrequency (0.0), _Attack (0.125), _Decay (0.125),
    _Glisson (0.0), _RndGlisson (0.0), _Morph (0.0), _RndMorph (0.0)
{
    setDefaults();
}

void apePulsaret::setDefaults()
{
    setLevel (0.5);
    setWidth (0.0);
    setPanningMode (0);
    setAttack (0.25);
    setDecay (0.25);
    setLength (85.0);
    setDensity (10.0);
    setRndLength (0.0);
    setRndDensity (0.0);
    setCps (440.0);
    setRndCps (0.0);
    setGlisson (0.0);
    setRndGlisson (0.0);
    setMorph (0.0);
    setRndMorph (0.0);
}

void apePulsaret::setAttack (double attack)        { _Attack = unit (attack) * 0.5; }
void apePulsaret::setDecay (double decay)          { _Decay  = unit (decay) * 0.5; }
void apePulsaret::setCps (double cps)              { if (cps == cps) _Frequency = cps; }
void apePulsaret::setRndCps (double semitones)     { if (semitones == semitones) _RndFrequency = semitones / 24.0; }
void apePulsaret::setGlisson (double semitones)    { if (semitones == semitones) _Glisson = semitones / 12.0; }
void apePulsaret::setRndGlisson (double semitones) { if (semitones == semitones) _RndGlisson = semitones / 12.0; }
void apePulsaret::setMorph (double morph)          { if (morph == morph) _Morph = morph < 0.0 ? 0.0 : (morph > 1.0 ? 1.0 : morph); }
void apePulsaret::setRndMorph (double amount)      { if (amount == amount) _RndMorph = amount; }

bool apePulsaret::prepareGrain (apeGrain& g)
{
    // numeri casuali nello stesso ordine del 2022: frequenza, glissando, morphing
    const double frequency = _Frequency.load (std::memory_order_relaxed) * pow (2.0, bipolar (_RndFrequency.load (std::memory_order_relaxed)));

    double glissando = _Glisson.load (std::memory_order_relaxed) + unipolar (_RndGlisson.load (std::memory_order_relaxed));
    if (glissando < 0.0)
        glissando *= 0.5;

    double morph = _Morph.load (std::memory_order_relaxed) + unipolar (_RndMorph.load (std::memory_order_relaxed));
    if (morph > 1.0) morph = 1.0 - (morph - 1.0);       // oltre 1 torna indietro
    if (! (morph >= 0.0)) morph = 0.0;
    if (morph > 1.0) morph = 1.0;

    if (! _wave.valid() || ! _win.valid() || ! _wave2.valid())
        return false;

    g.incr    = frequency / getAbsoluteSR();
    g.phi     = 0.0;
    g.glisson = glissando;
    g.morph   = morph;
    g.attack  = _Attack.load (std::memory_order_relaxed);
    g.decay   = _Decay.load (std::memory_order_relaxed);
    g.sus     = 1.0 - (g.attack + g.decay);
    return true;
}

bool apePulsaret::renderGrain (apeGrain& g, double* L, double* R, long n)
{
    return g.render2 (L, R, n, _wave, _win, _wave2, interp());
}

void apePulsaret::Processing (double* outL, double* outR, long sampleframes,
                              const apeTable& wave, const apeTable& win, const apeTable& wave2)
{
    if (! wave.valid() || ! win.valid() || ! wave2.valid())
    {
        memset (outL, 0, sizeof (double) * (size_t) sampleframes);
        memset (outR, 0, sizeof (double) * (size_t) sampleframes);
        return;
    }

    _wave  = wave;
    _win   = win;
    _wave2 = wave2;

    run (outL, outR, sampleframes);
}
