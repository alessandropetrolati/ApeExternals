//
//  apeGrain.h — un grano di suono
//
//  Alessandro Petrolati, apeSoft (prima versione: 21/03/2022). Riscritto nel 2026.
//
//  NUCLEO: questo file non conosce Max. Usa solo il C++ standard, quindi si puo'
//  provare da terminale e riusare in un plugin, in un'app, in un altro external.
//
//  Che cosa e' cambiato rispetto al 2022 (l'algoritmo e' lo stesso):
//  - il grano scrive direttamente nelle due uscite (prima: in un buffer di passaggio,
//    poi copiato e sommato per tutto il blocco);
//  - la scelta "con o senza interpolazione" e' fatta una volta per blocco, fuori dal
//    ciclo sui campioni (prima: uno switch per ogni campione);
//  - quando il grano finisce il ciclo si ferma (prima: continuava a scrivere zeri);
//  - tutti i valori hanno un valore iniziale e ogni indice resta dentro la tabella;
//  - le tabelle possono avere piu' canali (buffer~ stereo): si legge il primo.
//

#ifndef apeGrain_h
#define apeGrain_h

#include <cmath>

//! Una tabella di campioni: per Max e' il contenuto di un buffer~.
struct apeTable
{
    const float* data     = nullptr;   // campioni; con piu' canali sono alternati (S D S D ...)
    long         frames   = 0;         // quanti campioni per canale
    long         channels = 1;         // quanti canali
    double       sr       = 44100.0;   // frequenza di campionamento del file

    bool valid() const { return data != nullptr && frames > 0 && channels > 0; }
};

struct apeGrain
{
    // --- stato ---
    double phi     = 0.0;   // fase nella forma d'onda (0 - 1)
    double envPhi  = 0.0;   // fase nell'inviluppo (0 - 1): a 1 il grano e' finito
    // --- parametri, fissati quando il grano nasce ---
    double incr    = 0.0;   // passo della fase per ogni campione
    double envIncr = 1.0;   // passo dell'inviluppo per ogni campione
    double amp     = 0.0;
    double left    = 0.0;   // panorama
    double right   = 0.0;
    double glisson = 0.0;   // solo pulsaret
    double morph   = 0.0;   // solo pulsaret
    double attack  = 0.25;  // solo pulsaret: deformazione dell'inviluppo
    double decay   = 0.25;
    double sus     = 0.5;
    int    delay   = 0;     // campioni di attesa nel blocco in cui il grano nasce

    //! Grano semplice (ape.density~). "dir" e' 1 (avanti) o -1 (lettura al contrario).
    //! Somma "n" campioni in L e R. Ritorna false quando il grano e' finito.
    inline bool render (double* L, double* R, long n, const apeTable& wave, const apeTable& win, bool interp, double dir)
    {
        return interp ? renderT<true>  (L, R, n, wave, win, dir)
                      : renderT<false> (L, R, n, wave, win, dir);
    }

    //! Grano con deformazione dell'inviluppo, glissando e morphing fra due onde (ape.pulsaret~).
    inline bool render2 (double* L, double* R, long n, const apeTable& wave, const apeTable& win, const apeTable& wave2, bool interp)
    {
        return interp ? render2T<true>  (L, R, n, wave, win, wave2)
                      : render2T<false> (L, R, n, wave, win, wave2);
    }

private:
    //! Legge la tabella alla posizione "pos" (0 - 1). L'indice resta sempre dentro la tabella.
    template <bool Interp>
    static inline double read (const float* d, long frames, long ch, double pos)
    {
        const double t = pos * (double) frames;
        long i = (long) t;
        if (i >= frames) i = frames - 1;
        if (i < 0)       i = 0;

        if constexpr (Interp)
        {
            long nx = i + 1;
            if (nx >= frames) nx = 0;                      // come nel 2022: dopo l'ultimo si torna al primo
            const double a = d[i * ch], b = d[nx * ch];
            return a + (b - a) * (t - (double) i);
        }
        else
            return d[i * ch];
    }

    //! Riporta la fase fra 0 e 1.
    static inline double wrap (double p)
    {
        if (p >= 1.0)      p -= 1.0;
        else if (p < 0.0)  p += 1.0;
        if (! (p >= 0.0 && p < 1.0))                        // raro: passi piu' grandi di 1, valori non validi
        {
            p -= std::floor (p);
            if (! (p >= 0.0 && p < 1.0)) p = 0.0;
        }
        return p;
    }

    //! Quanti campioni si possono calcolare "di corsa", cioe' senza che il grano finisca e senza
    //! che la fase faccia il giro. "maxStep" e' il passo piu' grande (senza segno) della fase;
    //! "forward"/"backward" dicono in che verso puo' muoversi. Il conto e' prudente (due campioni in meno).
    static inline long safeRun (double p, double e, double eStep, double maxStep, bool forward, bool backward, long maxCount)
    {
        double m = (1.0 - e) / eStep;                                  // fine del grano
        if (maxStep > 0.0)
        {
            double room = 1.0;
            if (forward)  room = 1.0 - p;
            if (backward) room = (forward && room < p) ? room : p;
            const double r = room / maxStep;                           // giro della fase
            if (r < m) m = r;
        }
        m -= 2.0;
        if (! (m > 0.0)) return 0;
        return m < (double) maxCount ? (long) m : maxCount;
    }

    // Come sono fatti i due cicli qui sotto.
    // Nel ciclo di un grano, ogni campione aspetta la fase del campione prima: e' la catena che
    // decide la velocita'. Se nella catena ci sono anche i controlli ("il grano e' finito?",
    // "la fase ha fatto il giro?") il processore va piu' piano. Per questo il lavoro e' diviso:
    // - tratto DI CORSA: si calcola prima quanti campioni sono sicuri, poi un ciclo senza controlli;
    // - tratto CON CONTROLLI: pochi campioni vicino alla fine del grano o al giro della fase.
    // I conti sono gli stessi del 2022, nello stesso ordine: il suono non cambia.

    template <bool Interp>
    inline bool renderT (double* L, double* R, long n, const apeTable& wave, const apeTable& win, double dir)
    {
        // copie locali: il compilatore le tiene nei registri del processore
        double p = phi, e = envPhi;
        const double step = incr * dir, eStep = envIncr, a = amp, gl = left, gr = right;
        const float* w  = wave.data; const long wf = wave.frames, wc = wave.channels;
        const float* v  = win.data;  const long vf = win.frames,  vc = win.channels;
        const double absStep = std::fabs (step);
        const bool   canRun  = absStep < 0.0625 && eStep > 0.0 && p >= 0.0 && p < 1.0;
        bool alive = true;
        long k = 0;

        while (k < n)
        {
            if (canRun)                                                // tratto di corsa
            {
                const long m = safeRun (p, e, eStep, absStep, step > 0.0, step < 0.0, n - k);
                for (const long end = k + m; k < end; ++k)
                {
                    e += eStep;
                    p += step;
                    const double s = read<Interp> (w, wf, wc, p) * read<Interp> (v, vf, vc, e) * a;
                    L[k] += s * gl;
                    R[k] += s * gr;
                }
            }

            long end = canRun ? k + 4 : n;                             // tratto con controlli
            if (end > n) end = n;
            for (; k < end; ++k)
            {
                e += eStep;
                if (e >= 1.0) { alive = false; k = n; break; }

                p = wrap (p + step);
                const double s = read<Interp> (w, wf, wc, p) * read<Interp> (v, vf, vc, e) * a;
                L[k] += s * gl;
                R[k] += s * gr;
            }
        }

        phi = p; envPhi = e;
        return alive;
    }

    template <bool Interp>
    inline bool render2T (double* L, double* R, long n, const apeTable& wave, const apeTable& win, const apeTable& wave2)
    {
        double p = phi, e = envPhi;
        const double base = incr, eStep = envIncr, a = amp, gl = left, gr = right;
        const double att = attack, dec = decay, attSus = attack + sus, gliss = glisson;
        const double kAtt = 0.5 / att, kDec = 0.5 / dec;                // [2026] una moltiplicazione al posto di una divisione per campione
        const double m2 = morph, m1 = 1.0 - morph;
        const float* w  = wave.data;  const long wf = wave.frames,  wc = wave.channels;
        const float* w2 = wave2.data; const long w2f = wave2.frames, w2c = wave2.channels;
        const float* v  = win.data;   const long vf = win.frames,   vc = win.channels;

        // con il glissando il passo va da "base" (inizio del grano) a "base * (1 + gliss)" (fine)
        const double s0 = base, s1 = base * (1.0 + gliss);
        const double maxStep = std::fabs (s0) > std::fabs (s1) ? std::fabs (s0) : std::fabs (s1);
        const bool   forward = s0 > 0.0 || s1 > 0.0, backward = s0 < 0.0 || s1 < 0.0;
        const bool   canRun  = maxStep < 0.0625 && eStep > 0.0 && p >= 0.0 && p < 1.0;
        bool alive = true;
        long k = 0;

        // un campione: deformazione dell'inviluppo, lettura delle due onde, somma nelle uscite
        #define APE_PULSAR_SAMPLE                                                                   \
            double ed;                                                                              \
            if (e < att)          ed = e * kAtt;                   /* attacco */                    \
            else if (e < attSus)  ed = 0.5;                        /* centro della finestra */      \
            else                  ed = 1.0 - (1.0 - e) * kDec;     /* decadimento */                \
            if (! (ed >= 0.0)) ed = 0.0;                           /* anche per decay = 0 */        \
            const double s = (m1 * read<Interp> (w, wf, wc, p) + m2 * read<Interp> (w2, w2f, w2c, p)) \
                             * read<Interp> (v, vf, vc, ed) * a;                                    \
            L[k] += s * gl;                                                                         \
            R[k] += s * gr;

        while (k < n)
        {
            if (canRun)                                                // tratto di corsa
            {
                const long m = safeRun (p, e, eStep, maxStep, forward, backward, n - k);
                for (const long end = k + m; k < end; ++k)
                {
                    e += eStep;
                    p += base * (1.0 + e * gliss);                     // glissando: il passo cambia lungo il grano
                    APE_PULSAR_SAMPLE
                }
            }

            long end = canRun ? k + 4 : n;                             // tratto con controlli
            if (end > n) end = n;
            for (; k < end; ++k)
            {
                e += eStep;
                if (e >= 1.0) { alive = false; k = n; break; }

                const double step = base * (1.0 + e * gliss);
                if (! (step > 1.0)) p += step;                         // come nel 2022: un passo oltre 1 e' saltato
                p = wrap (p);
                APE_PULSAR_SAMPLE
            }
        }
        #undef APE_PULSAR_SAMPLE

        phi = p; envPhi = e;
        return alive;
    }
};

#endif
