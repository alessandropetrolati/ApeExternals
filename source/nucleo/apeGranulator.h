//
//  apeGranulator.h — la parte comune ai due granulatori
//
//  Alessandro Petrolati, apeSoft. 2026.
//
//  NUCLEO: non conosce Max.
//
//  Qui stanno: le voci (i grani), il metronomo che le fa partire, i numeri casuali,
//  i parametri comuni. apeDensity e apePulsaret aggiungono solo quello che li distingue:
//  come si prepara un grano e come lo si calcola.
//
//  Due thread usano questo oggetto:
//  - il thread dei messaggi chiama i metodi set...() e grain();
//  - il thread audio chiama Processing().
//  I parametri sono std::atomic: si leggono e si scrivono senza blocchi (su Apple Silicon
//  costano come una variabile normale). Le voci le tocca SOLO il thread audio.
//  Nel thread audio non si chiede mai memoria: tutto e' creato nel costruttore.
//

#ifndef apeGranulator_h
#define apeGranulator_h

#include "apeGrain.h"
#include <atomic>
#include <vector>

class apeGranulator
{
public:
    apeGranulator (int maxVoices, double sr);
    virtual ~apeGranulator() = default;

    void setToggle (int toggle);              //!< 1 = il metronomo fa partire i grani
    void setOverlap (int maxOverlap);         //!< quanti grani insieme, al massimo (1 - maxVoices)
    void setLevel (double volume);
    void setWidth (double width);             //!< 0 - 1
    void setPanningMode (int mode);           //!< 0 = casuale dentro "width", 1 = manuale
    void setLength (double ms);               //!< durata del grano, millisecondi
    void setDensity (double gps);             //!< grani al secondo
    void setRndLength (double amount);        //!< 0 - 1, parte casuale della durata
    void setRndDensity (double amount);       //!< 0 - 1, parte casuale della densita'
    void setInterp (int interp);              //!< 1 = interpolazione lineare
    void setAbsoluteSR (double sr);
    void setSeed (int seed);                  //!< per le prove: numeri casuali ripetibili
    void grain();                             //!< chiede un grano subito (lo fa partire il thread audio)

    double getAbsoluteSR() const        { return _SR.load (std::memory_order_relaxed); }
    int    getOverlappingFactor() const { return _overlap.load (std::memory_order_relaxed); }
    int    getMaxVoices() const         { return (int) _voices.size(); }
    int    getNumOscActive() const      { return _numActivePublic.load (std::memory_order_relaxed); }
    double getGrainLength() const       { return _lastLength.load (std::memory_order_relaxed); }

protected:
    //! Il lavoro di un blocco: azzera le uscite, fa partire i grani, li somma. Solo thread audio.
    void run (double* L, double* R, long n);

    //! La classe derivata mette nel grano frequenza, fase e il resto. false = non si puo' (tabelle non valide).
    virtual bool prepareGrain (apeGrain& g) = 0;
    //! La classe derivata calcola "n" campioni del grano. false = grano finito.
    virtual bool renderGrain (apeGrain& g, double* L, double* R, long n) = 0;

    double unipolar (double amp);             //!< casuale fra 0 e amp
    double bipolar (double amp);              //!< casuale fra -amp e amp

    bool interp() const { return _interp.load (std::memory_order_relaxed); }

private:
    void newGrain (int delay);

    // --- parametri (thread dei messaggi -> thread audio) ---
    std::atomic<double> _SR, _Volume, _Width, _Length, _Density, _RndLength, _RndDensity, _lastLength;
    std::atomic<bool>   _Start, _PanningMode, _interp;
    std::atomic<int>    _overlap, _grainRequests, _numActivePublic, _newSeed;
    std::atomic<bool>   _seedChanged;

    // --- stato del thread audio ---
    std::vector<apeGrain> _voices;            // tutte le voci, una accanto all'altra in memoria
    std::vector<int>      _active;            // indici delle voci che suonano
    std::vector<int>      _free;              // indici delle voci libere
    int           _numActive = 0;
    int           _numFree   = 0;
    unsigned long _thres = 0;                 // campioni fra un grano e il successivo
    unsigned long _count = 0;                 // campioni passati dall'ultimo grano
    int           _seedRnd;
};

#endif
