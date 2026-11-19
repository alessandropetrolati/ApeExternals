//
//  apeDensity.h — granulatore di un file audio (il nucleo di ape.density~)
//
//  Alessandro Petrolati, apeSoft (prima versione: 21/03/2022). Riscritto nel 2026.
//  NUCLEO: non conosce Max.
//

#ifndef apeDensity_h
#define apeDensity_h

#include "apeGranulator.h"

class apeDensity : public apeGranulator
{
public:
    apeDensity (int maxOverlap = 50, double sr = 44100.0);

    void setDefaults();

    //! Trasposizione in semitoni: 0 = nessuna, 12 = ottava sopra, -6 = sei semitoni sotto
    void setPitch (double semitones);
    void setRndPitch (double semitones);
    //! Punto del file da cui parte il grano, 0 - 1
    void setPhase (double ph);
    //! Lo stesso punto, in millisecondi
    void setSamplePosition (double ms);
    //! 1 = legge il file al contrario
    void setInvert (int mode);

    //! Un blocco audio. "wave" e' il file da granulare, "win" la finestra (inviluppo del grano).
    //! Se una tabella non e' valida, le uscite sono silenzio.
    void Processing (double* outL, double* outR, long sampleframes, const apeTable& wave, const apeTable& win);

protected:
    bool prepareGrain (apeGrain& g) override;
    bool renderGrain (apeGrain& g, double* L, double* R, long n) override;

private:
    std::atomic<double> _Frequency, _RndFrequency, _Phase, _dir;
    std::atomic<double> _waveSeconds;      // durata del file, per setSamplePosition
    apeTable _wave, _win;                  // le tabelle del blocco in corso (thread audio)
};

#endif
