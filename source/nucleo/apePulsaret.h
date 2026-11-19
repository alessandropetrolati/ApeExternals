//
//  apePulsaret.h — sintesi pulsar con due forme d'onda (il nucleo di ape.pulsaret~)
//
//  Alessandro Petrolati, apeSoft (prima versione: 21/03/2022). Riscritto nel 2026.
//  NUCLEO: non conosce Max.
//

#ifndef apePulsaret_h
#define apePulsaret_h

#include "apeGranulator.h"

class apePulsaret : public apeGranulator
{
public:
    apePulsaret (int maxOverlap = 50, double sr = 44100.0);

    void setDefaults();

    void setAttack (double attack);        //!< 0 - 1: parte del grano occupata dall'attacco
    void setDecay (double decay);          //!< 0 - 1: parte occupata dal decadimento
    void setCps (double cps);              //!< frequenza del pulsaret, Hz
    void setRndCps (double semitones);
    void setGlisson (double semitones);    //!< glissando dentro il grano
    void setRndGlisson (double semitones);
    void setMorph (double morph);          //!< 0 = prima onda, 1 = seconda onda
    void setRndMorph (double amount);

    //! Un blocco audio. "wave" e "wave2" sono le due forme d'onda, "win" la finestra.
    void Processing (double* outL, double* outR, long sampleframes,
                     const apeTable& wave, const apeTable& win, const apeTable& wave2);

protected:
    bool prepareGrain (apeGrain& g) override;
    bool renderGrain (apeGrain& g, double* L, double* R, long n) override;

private:
    std::atomic<double> _Frequency, _RndFrequency, _Attack, _Decay, _Glisson, _RndGlisson, _Morph, _RndMorph;
    apeTable _wave, _win, _wave2;
};

#endif
