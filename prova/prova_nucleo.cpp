// prova_nucleo.cpp — prova dei NUCLEI dei due external (apeDensity, apePulsaret) senza Max.
// Il nucleo non usa niente di Max: si compila da terminale con il solo C++ standard.
//
//   cd prova
//   c++ -std=c++17 -O3 -I../source/nucleo prova_nucleo.cpp ../source/nucleo/*.cpp -o prova_nucleo && ./prova_nucleo
//
// Con i controlli di memoria (consigliato dopo ogni modifica):
//   c++ -std=c++17 -g -fsanitize=address,undefined -I../source/nucleo prova_nucleo.cpp ../source/nucleo/*.cpp -o prova_nucleo && ./prova_nucleo
#include "apeDensity.h"
#include "apePulsaret.h"
#include <chrono>
#include <cstdio>
#include <limits>
#include <vector>

static const double SR = 48000.0;
static int errori = 0;

static void esito (bool ok, const char* testo) { std::printf ("%s  %s\n", ok ? "ok    " : "ERRORE", testo); if (! ok) ++errori; }

// livello in dB di una frequenza dentro un suono
static double livello (const std::vector<double>& x, double hz)
{
    double re = 0, im = 0; const size_t n = x.size();
    for (size_t k = 0; k < n; ++k) { const double a = 2.0 * M_PI * hz * (double) k / SR; re += x[k] * std::cos (a); im -= x[k] * std::sin (a); }
    return 20.0 * std::log10 (2.0 * std::sqrt (re * re + im * im) / (double) n + 1e-12);
}

// picco di un suono; "non finito" se dentro c'e' un valore non valido
static double picco (const std::vector<double>& x)
{
    double m = 0;
    for (double v : x) { if (! (v == v) || std::fabs (v) > 1e300) return std::numeric_limits<double>::infinity(); m = std::fmax (m, std::fabs (v)); }
    return m;
}

// Al posto dei buffer~ di Max: tabelle fatte qui
struct Banco
{
    std::vector<float> seno, stereo, seno44, quadra, finestra, uno;
    apeTable tSeno, tStereo, tSeno44, tQuadra, tFinestra, tUno, tVuota;

    Banco() : seno (48000), stereo (96000), seno44 (44100), quadra (2048), finestra (512), uno (1, 0.5f)
    {
        for (int n = 0; n < 48000; ++n) { seno[n] = (float) std::sin (2.0 * M_PI * 440.0 * n / SR); stereo[2 * n] = seno[n]; stereo[2 * n + 1] = 0.f; }
        for (int n = 0; n < 44100; ++n) seno44[n] = (float) std::sin (2.0 * M_PI * 440.0 * n / 44100.0);
        for (int n = 0; n < 2048; ++n)  quadra[n] = n < 1024 ? 0.5f : -0.5f;
        for (int n = 0; n < 512; ++n)   finestra[n] = (float) (0.5 - 0.5 * std::cos (2.0 * M_PI * n / 511.0));
        tSeno     = { seno.data(),     48000, 1, SR };          // 1 secondo, 440 Hz, mono
        tStereo   = { stereo.data(),   48000, 2, SR };          // lo stesso, stereo (destra muta)
        tSeno44   = { seno44.data(),   44100, 1, 44100.0 };     // lo stesso, file a 44,1 kHz
        tQuadra   = { quadra.data(),   2048,  1, SR };          // un ciclo di onda quadra
        tFinestra = { finestra.data(), 512,   1, SR };          // finestra di Hanning
        tUno      = { uno.data(),      1,     1, SR };          // tabella di un solo campione
    }
};

// fa lavorare un granulatore per "secondi", a blocchi (come perform64 di Max)
template <typename Blocco>
static std::vector<double> suona (double secondi, int blocco, Blocco&& unBlocco)
{
    const long N = (long) (SR * secondi) / blocco * blocco;
    std::vector<double> L ((size_t) N), R ((size_t) N);
    for (long p = 0; p < N; p += blocco) unBlocco (&L[(size_t) p], &R[(size_t) p], (long) blocco);
    return L;
}

int main()
{
    Banco b;
    const double nan = std::numeric_limits<double>::quiet_NaN();

    std::printf ("--- apeDensity\n");
    {
        apeDensity g (64, SR); g.setSeed (1); g.setLength (200); g.setDensity (20); g.setToggle (1);
        auto x = suona (1.0, 480, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        esito (livello (x, 440) > -3.0, "file mono: il tono a 440 Hz esce");
    }
    {
        apeDensity g (64, SR); g.setSeed (1); g.setLength (200); g.setDensity (20); g.setToggle (1);
        auto x = suona (1.0, 480, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tStereo, b.tFinestra); });
        esito (livello (x, 440) > -3.0, "buffer stereo: si legge il primo canale, il tono resta a 440 Hz");
    }
    {
        apeDensity g (64, SR); g.setSeed (1); g.setLength (200); g.setDensity (20); g.setToggle (1);
        auto x = suona (1.0, 480, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno44, b.tFinestra); });
        esito (livello (x, 440) > -3.0, "file a 44,1 kHz, motore a 48 kHz: il tono resta a 440 Hz");
    }
    {
        apeDensity g (64, SR); g.setSeed (1); g.setPitch (12); g.setLength (200); g.setDensity (20); g.setToggle (1);
        auto x = suona (1.0, 480, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        esito (livello (x, 880) > -3.0, "setPitch 12: un'ottava sopra, 880 Hz");
    }
    {
        apeDensity g (64, SR); g.grain();                                       // senza "start"
        auto x = suona (0.2, 512, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        esito (picco (x) > 0.1 && g.getNumOscActive() == 0, "grain prima di start: un grano si sente e poi la voce e' libera");
    }
    {
        apeDensity g (64, SR); g.setDensity (5000); g.setLength (500); g.setToggle (1);
        suona (1.0, 512, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        const int tutte = g.getNumOscActive();
        g.setOverlap (8);                                                       // con l'audio "acceso"
        suona (1.0, 512, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        esito (tutte == 64 && g.getNumOscActive() == 8, "overlap: 64 voci, poi limite a 8");
    }
    {
        apeDensity g (64, SR); g.setToggle (1);
        g.setRndLength (4); g.setRndDensity (9); g.setPitch (120); g.setRndPitch (48); g.setLength (-5); g.setPhase (1); g.setInvert (1); g.setInterp (1);
        auto x = suona (5.0, 512, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        g.setLength (nan); g.setDensity (nan); g.setPitch (nan); g.setPhase (nan); g.setWidth (nan); g.setLevel (nan); g.setSamplePosition (nan);
        auto y = suona (5.0, 512, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        esito (picco (x) < 100.0 && picco (y) < 100.0, "parametri fuori scala e non validi: nessun errore, uscita finita");
    }
    {
        // ogni prova con un granulatore nuovo: si misura anche l'inizio, quando i grani non si sovrappongono ancora
        auto prova = [&] (int blocco, const apeTable& onda, const apeTable& fin)
        {
            apeDensity g (64, SR); g.setSeed (1); g.setToggle (1); g.setDensity (300); g.setLength (100); g.setInterp (1);
            return picco (suona (2.0, blocco, [&] (double* l, double* r, long n) { g.Processing (l, r, n, onda, fin); }));
        };
        const double x = prova (8192, b.tSeno, b.tFinestra), y = prova (1, b.tSeno, b.tFinestra), z = prova (512, b.tUno, b.tUno), v = prova (512, b.tVuota, b.tFinestra);
        esito (x > 0.1 && std::fabs (x - y) < 1e-9 && z < 100.0 && v == 0.0, "blocchi di 8192 e di 1 campione (stesso picco), tabella di 1 campione, tabella mancante (silenzio)");
    }

    std::printf ("--- apePulsaret\n");
    {
        apePulsaret g (1024, SR); g.setSeed (1); g.setCps (440); g.setToggle (1);       // 10 grani al secondo, di 85 ms
        auto x = suona (1.0, 480, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tQuadra, b.tFinestra, b.tQuadra); });
        esito (picco (x) > 0.2 && livello (x, 440) > livello (x, 880) + 20.0, "setCps 440: onda quadra a 440 Hz (fondamentale forte, 880 Hz assente)");
    }
    {
        apePulsaret g (1024, SR); g.grain();                                    // senza "start"
        auto x = suona (0.2, 512, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tQuadra, b.tFinestra, b.tQuadra); });
        esito (picco (x) > 0.1 && g.getNumOscActive() == 0, "grain prima di start: un grano si sente e poi la voce e' libera");
    }
    {
        apePulsaret g (1024, SR); g.setToggle (1);
        g.setAttack (0); g.setDecay (0); g.setGlisson (-300); g.setRndGlisson (500); g.setRndMorph (50); g.setCps (1e9); g.setRndCps (1e4);
        g.setDensity (1e7); g.setLength (1e6); g.setRndLength (7); g.setInterp (1);
        auto x = suona (5.0, 512, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tQuadra, b.tFinestra, b.tStereo); });
        esito (picco (x) < 1e6 && g.getNumOscActive() == 1024, "parametri fuori scala: nessun errore, tutte le 1024 voci occupate");
    }

    std::printf ("--- carico (un solo core, senza Max)\n");
    {
        apeDensity g (64, SR); g.setDensity (333); g.setLength (150); g.setInterp (1); g.setToggle (1);
        const auto t0 = std::chrono::steady_clock::now();
        suona (60.0, 64, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tSeno, b.tFinestra); });
        const double s = std::chrono::duration<double> (std::chrono::steady_clock::now() - t0).count();
        std::printf ("        apeDensity,  %2d voci, blocco 64: %.2f %% di un core\n", g.getNumOscActive(), s / 60.0 * 100.0);
    }
    {
        apePulsaret g (1024, SR); g.setDensity (700); g.setLength (120); g.setCps (55); g.setGlisson (-9); g.setMorph (0.5); g.setInterp (1); g.setToggle (1);
        const auto t0 = std::chrono::steady_clock::now();
        suona (60.0, 64, [&] (double* l, double* r, long n) { g.Processing (l, r, n, b.tQuadra, b.tFinestra, b.tSeno); });
        const double s = std::chrono::duration<double> (std::chrono::steady_clock::now() - t0).count();
        std::printf ("        apePulsaret, %2d voci, blocco 64: %.2f %% di un core\n", g.getNumOscActive(), s / 60.0 * 100.0);
    }

    std::printf (errori == 0 ? "\nTutte le prove sono riuscite.\n" : "\nProve fallite: %d\n", errori);
    return errori;
}
