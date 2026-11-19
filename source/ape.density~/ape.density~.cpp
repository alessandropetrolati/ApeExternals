//
//  ape.density~.cpp — l'INVOLUCRO: tutto quello che parla con Max
//
//  Created by Alessandro Petrolati on 21/03/2022. Riscritto nel 2026.
//  Copyright 2026 Prof. All rights reserved.
//
//  Il suono lo fa il NUCLEO (source/nucleo/apeDensity), che non conosce Max.
//  Qui ci sono solo: la classe dell'oggetto, i messaggi, i buffer~, il collegamento all'audio.
//
// ============================================================================================
//  COME SI LEGGE QUESTO FILE (guida all'SDK di Max)
// ============================================================================================
//
//  Un external e' una libreria dinamica: un file .mxo (macOS) o .mxe64 (Windows) che Max carica
//  mentre e' in funzione. Max non sa niente del nostro codice: conosce solo un nome, ext_main().
//  Da li' in poi siamo noi a dirgli tutto, chiamando le funzioni dell'SDK.
//
//  La vita di un external ha cinque momenti. In questo file si trovano in quest'ordine:
//
//    1. AUDIO      density_perform64  un blocco di campioni            (thread audio, molte volte al secondo)
//                  density_dsp64      si accende l'audio               (una volta a ogni accensione)
//    2. BUFFER~    setSample, setWindowing, setbuf, notify, dblclick
//    3. MESSAGGI   start, setDensity, ...                              (thread principale o scheduler)
//    4. OGGETTO    density_new        nasce un oggetto nella patch
//                  density_free       l'oggetto viene cancellato
//    5. CLASSE     ext_main           Max carica il file .mxo          (una volta sola)
//
//  Max li usa al contrario: prima ext_main (5), poi density_new (4), poi il resto.
//  Il file e' scritto "dal basso": in C una funzione deve essere scritta prima di essere usata.
//  Per capire, conviene leggere dal fondo: ext_main, poi density_new, poi density_dsp64.
//
//  DA DOVE ARRIVA L'SDK. Non si installa: e' una cartella, max-sdk-base/c74support, con dentro
//    max-includes/   le intestazioni di Max (ext.h ...) e l'elenco delle sue funzioni
//    msp-includes/   le intestazioni dell'audio (z_dsp.h, ext_buffer.h) e MaxAudioAPI.framework
//  Il progetto la usa in tre punti (vedi Build Settings del target, oppure CMakeLists.txt):
//    - Header Search Paths: dove il compilatore trova i file degli #include qui sotto;
//    - Other Linker Flags, file c74_linker_flags.txt: l'elenco delle funzioni di Max. Dice al
//      linker "queste funzioni non sono qui dentro: le troverai dentro Max, quando l'oggetto
//      viene caricato". E' un collegamento DINAMICO, e al contrario del solito: e' il modulo
//      che usa le funzioni del programma che lo ospita;
//    - Other Linker Flags, -framework MaxAudioAPI: lo stesso, per le funzioni dell'audio.
//
//  I NOMI DELL'SDK. I tipi cominciano con t_ (t_object, t_symbol, t_atom, t_class).
//    t_symbol  una parola di Max ("start", il nome di un buffer~). Si ottiene con gensym("parola").
//    t_atom    un elemento di un messaggio: puo' essere un intero, un decimale o un simbolo.
//    t_object  un oggetto qualsiasi di Max. Il nostro oggetto "e' un" t_object (vedi la struttura).
//

// ---- Le intestazioni dell'SDK ---------------------------------------------------------------
#include "ext.h"            // la base: t_object, t_symbol, t_atom, post(), outlet_int(), gensym() ...
#include "ext_obex.h"       // le classi "moderne": class_new, class_addmethod, object_alloc, object_free
#include "z_dsp.h"          // MSP, cioe' l'audio: t_pxobject, dsp_setup, dsp_free, class_dspinit, sys_getsr
#include "ext_buffer.h"     // l'accesso ai buffer~: t_buffer_ref, buffer_locksamples ...

// ---- Il nostro codice -----------------------------------------------------------------------
#include "apeDensity.h"     // il nucleo. Da qui in giu' e' l'UNICO punto di contatto con il granulatore
#include <cstring>          // memset
#include <cstdio>           // snprintf

#define APE_MSG "ape.density~ 1.2 by Alessandro Petrolati © 2026 All Rigth Reserved www.apesoft.it"
#define MAXGRAINS (64)   // quante voci al massimo: sono create una volta sola, alla nascita dell'oggetto

// La CLASSE: la "scheda" che descrive a Max il nostro tipo di oggetto (nome, messaggi, grandezza).
// Ce n'e' una sola per tutto il programma; la crea ext_main. "static": visibile solo in questo file.
static t_class *density_class = NULL;

// L'OGGETTO: i dati di UNA copia di [ape.density~] nella patch. Dieci oggetti = dieci strutture.
// Regola dell'SDK: il primo campo deve essere t_pxobject (per un oggetto senza audio: t_object).
// Max guarda solo quel primo pezzo, che contiene entrate, uscite e dati dell'audio; il resto e' nostro.
// E' l'ereditarieta' fatta in C: "t_density e' un t_pxobject, che e' un t_object".
typedef struct _density {
    t_pxobject    l_obj;        // prima cosa nella struttura: e' un oggetto MSP
    apeDensity   *_granulator;  // il nucleo: un oggetto C++, creato con new in density_new
    t_buffer_ref *samplebuf;    // riferimento al buffer~ con il file da granulare
    t_buffer_ref *windowbuf;    // riferimento al buffer~ con la finestra (inviluppo del grano)
    t_symbol     *samplename;   // i due nomi: servono a density_notify e a density_dsp64
    t_symbol     *windowname;
    void         *m_outlet1;    // uscita di destra: numero di grani attivi
} t_density;

// Tutte le funzioni qui sotto sono "static": fuori da questo file non servono a nessuno.
// Max le raggiunge con gli indirizzi che gli diamo in ext_main (class_addmethod), non con il nome.
// L'unica funzione che deve avere un nome pubblico e' ext_main.


// ============================================================================================
//  1. AUDIO
// ============================================================================================

// Un blocco audio: la funzione "perform". Max la chiama dal THREAD AUDIO, una volta per blocco
// (con 64 campioni a 48 kHz: 750 volte al secondo). Qui dentro NON si puo': scrivere nella
// finestra di Max (post), chiedere memoria (new, malloc), aspettare (lock lenti, file).
// Il "64" nel nome: dal 2011 (Max 6) l'audio di Max e' fatto di numeri double, a 64 bit.
//
//   x             il nostro oggetto
//   dsp64         la catena audio di Max (qui non serve)
//   ins, numins   le entrate di segnale:  ins[0] ... ins[numins - 1], ognuna un vettore di campioni
//   outs, numouts le uscite di segnale:   outs[0] = la piu' a sinistra
//   sampleframes  quanti campioni in questo blocco (il "signal vector size")
//   flags, userparam  non usati (userparam e' il valore dato in dsp_add64)
static void density_perform64(t_density *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    // [2026] prima di tutto silenzio su tutte le uscite: Max non le azzera per noi.
    // Se usciamo senza scrivere, in uscita resta quello che c'era in memoria: rumore.
    for (long c = 0; c < numouts; c++)
        memset(outs[c], 0, sizeof(double) * sampleframes);

    if (numouts < 2 || !x->samplebuf || !x->windowbuf)
        return;

    // Dal riferimento (t_buffer_ref) all'oggetto buffer~ vero (t_buffer_obj).
    // Va chiesto a OGNI blocco: il buffer~ puo' essere stato cancellato un attimo fa.
    // NULL = in questo momento non esiste un buffer~ con quel nome.
    t_buffer_obj *sample = buffer_ref_getobject(x->samplebuf);
    t_buffer_obj *window = buffer_ref_getobject(x->windowbuf);
    if (!sample || !window)
        return;                                          // il buffer~ non esiste (ancora)

    // "lock": finche' non li rilasciamo, Max non sposta e non cancella i campioni
    // (per esempio se nella patch arriva un "read" o un "sizeinsamps").
    // I campioni di un buffer~ sono float, a 32 bit, anche se l'audio di Max e' a 64.
    // Con piu' canali sono alternati: S D S D ...  Puo' tornare NULL (buffer~ vuoto, o occupato).
    float *wavetable = buffer_locksamples(sample);
    float *wintable  = buffer_locksamples(window);

    if (wavetable && wintable) {
        // [2026] al nucleo diciamo anche quanti canali ha il buffer~ e la frequenza del file.
        // apeTable e' una struttura del NUCLEO: qui si traduce dal mondo di Max al mondo del nucleo.
        apeTable wave, win;
        wave.data = wavetable;  wave.frames = (long) buffer_getframecount(sample);
        wave.channels = (long) buffer_getchannelcount(sample);  wave.sr = buffer_getsamplerate(sample);
        win.data  = wintable;   win.frames  = (long) buffer_getframecount(window);
        win.channels  = (long) buffer_getchannelcount(window);  win.sr  = buffer_getsamplerate(window);

        // tutta la sintesi e' in questa riga
        x->_granulator->Processing(outs[0], outs[1], sampleframes, wave, win);
    }

    // ogni lock riuscito va rilasciato, sempre, anche se non abbiamo suonato
    if (wavetable) buffer_unlocksamples(sample);
    if (wintable)  buffer_unlocksamples(window);
}

// Max la chiama quando si accende l'audio (e ogni volta che la catena audio viene ricostruita:
// un collegamento cambiato, un'altra frequenza di campionamento). Qui si conoscono per la prima
// volta frequenza di campionamento e grandezza del blocco: e' il posto per preparare il nucleo.
//
//   count          per ogni entrata e uscita: 1 se c'e' un cavo di segnale collegato
//   samplerate     frequenza di campionamento
//   maxvectorsize  il blocco piu' grande che arrivera' a perform64
static void density_dsp64(t_density *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->_granulator->setAbsoluteSR(samplerate);

    // Si cercano di nuovo i due buffer~ per nome: cosi' l'oggetto trova anche un buffer~
    // creato dopo di lui. buffer_ref_set con lo stesso nome vuol dire "guarda di nuovo".
    if (x->samplebuf && x->samplename) buffer_ref_set(x->samplebuf, x->samplename);
    if (x->windowbuf && x->windowname) buffer_ref_set(x->windowbuf, x->windowname);

    // "Aggiungi alla catena audio la mia funzione perform". Non e' una chiamata diretta:
    // si manda all'oggetto dsp64 il messaggio "dsp_add64", come farebbe un cavo in una patch.
    // Gli ultimi due valori sono "flags" (0) e "userparam" (NULL), che ritroviamo in perform64.
    object_method(dsp64, gensym("dsp_add64"), x, density_perform64, 0, NULL);
}


// ============================================================================================
//  2. BUFFER~
// ============================================================================================
//
// Un external non tiene un puntatore al buffer~: l'utente puo' cancellarlo in ogni momento.
// Tiene un RIFERIMENTO (t_buffer_ref) legato a un NOME. Il riferimento sa se, in questo
// momento, esiste un buffer~ con quel nome, e lo segue se viene creato dopo.
//   buffer_ref_new      crea il riferimento (una volta)
//   buffer_ref_set      gli fa seguire un altro nome
//   buffer_ref_notify   gli passa gli avvisi di Max (vedi density_notify)
//   object_free         lo distrugge (in density_free)

static void density_setSample(t_density *x, t_symbol *s)
{
    x->samplename = s;
    if (!x->samplebuf) x->samplebuf = buffer_ref_new((t_object *)x, s);
    else               buffer_ref_set(x->samplebuf, s);
}

static void density_setWindowing(t_density *x, t_symbol *s)
{
    x->windowname = s;
    if (!x->windowbuf) x->windowbuf = buffer_ref_new((t_object *)x, s);
    else               buffer_ref_set(x->windowbuf, s);
}

// "setbuf file finestra": anche gli argomenti dell'oggetto passano di qui.
// E' un messaggio di tipo A_GIMME ("dammi tutto"): Max non controlla gli argomenti, ce li passa
// come sono. s = la parola del messaggio, ac = quanti argomenti, av = gli argomenti (t_atom).
// Con A_GIMME il controllo del tipo tocca a noi: atom_gettype, poi atom_getsym.
static void density_setbuf(t_density *x, t_symbol *s, long ac, t_atom *av)
{
    if (ac > 0 && atom_gettype(av) == A_SYM)     density_setSample(x, atom_getsym(av));
    if (ac > 1 && atom_gettype(av + 1) == A_SYM) density_setWindowing(x, atom_getsym(av + 1));
}

// Max avvisa il nostro oggetto quando succede qualcosa a un buffer~ che stiamo seguendo
// (e' stato modificato, cancellato ...). L'avviso va passato al riferimento, con buffer_ref_notify.
//
// ATTENZIONE, con piu' di un buffer~: l'avviso riguarda UN buffer~, e va passato SOLO al
// riferimento di quel buffer~. Se lo si passa a tutti, tutti i riferimenti finiscono per
// guardare lo stesso buffer~ (errore fatto e corretto il 20.11.2026: l'oggetto leggeva la
// finestra al posto del suono). Gli esempi dell'SDK hanno un solo buffer~ e non lo mostrano.
// Per sapere di chi parla l'avviso si chiede il nome a chi lo manda ("sender"):
// se non e' un buffer~, o non e' uno dei nostri, non si fa niente.
static t_max_err density_notify(t_density *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if (!sender)
        return MAX_ERR_NONE;

    t_symbol *name = (t_symbol *) object_method((t_object *)sender, gensym("getname"));

    if (name && name == x->samplename && x->samplebuf) buffer_ref_notify(x->samplebuf, s, msg, sender, data);
    if (name && name == x->windowname && x->windowbuf) buffer_ref_notify(x->windowbuf, s, msg, sender, data);
    return MAX_ERR_NONE;
}

// doppio clic sull'oggetto: apre i due buffer~ (la finestra con la forma d'onda)
static void density_dblclick(t_density *x)
{
    if (x->samplebuf) buffer_view(buffer_ref_getobject(x->samplebuf));
    if (x->windowbuf) buffer_view(buffer_ref_getobject(x->windowbuf));
}


// ============================================================================================
//  3. MESSAGGI
// ============================================================================================
//
// Una funzione per ogni messaggio. Max le chiama dal thread principale o dallo scheduler,
// non da dentro perform64: qui si puo' scrivere nella finestra di Max e mandare valori dalle uscite.
// Il primo argomento e' sempre l'oggetto; gli altri dipendono dal tipo dichiarato in ext_main:
//   A_LONG  -> long        A_FLOAT -> double        A_SYM -> t_symbol *
// Il nucleo riceve il valore mentre il thread audio lavora: per questo i suoi parametri
// sono std::atomic (vedi apeGranulator.h).
//
// object_error scrive in rosso nella finestra di Max; il doppio clic sulla riga porta all'oggetto.

static void density_start(t_density *x, long n)
{
    if (n < 0 || n > 1) object_error((t_object *)x, "start message: 0 = off; 1 = on");
    else                x->_granulator->setToggle((int) n);
}

static void density_interp(t_density *x, long n)
{
    if (n < 0 || n > 1) object_error((t_object *)x, "interp message: 0 = no interp; 1 = two point interp.");
    else                x->_granulator->setInterp((int) n);
}

static void density_overlap(t_density *x, long n)
{
    if (n < 1 || n > MAXGRAINS) object_error((t_object *)x, "overlap message: from 1 to %d", MAXGRAINS);
    else                        x->_granulator->setOverlap((int) n);
}

static void density_reverse(t_density *x, long n)
{
    if (n < 0 || n > 1) object_error((t_object *)x, "reverse message: 0 = normal; 1 = read backward samples");
    else                x->_granulator->setInvert((int) n);
}

static void density_setPanningMode(t_density *x, long n)
{
    if (n < 0 || n > 1) object_error((t_object *)x, "setPanningMode message: 0 = random width (use setWidth) 1 = manual (0 = L 1 = R)");
    else                x->_granulator->setPanningMode((int) n);
}

static void density_setLevel(t_density *x, double n)          { x->_granulator->setLevel(n); }
static void density_setWidth(t_density *x, double n)          { x->_granulator->setWidth(n); }
static void density_setLength(t_density *x, double n)         { x->_granulator->setLength(n); }
static void density_setRndLength(t_density *x, double n)      { x->_granulator->setRndLength(n); }
static void density_setDensity(t_density *x, double n)        { if (n > 0.00001) x->_granulator->setDensity(n); }
static void density_setRndDensity(t_density *x, double n)     { x->_granulator->setRndDensity(n); }
static void density_setPitch(t_density *x, double n)          { if (n < 128) x->_granulator->setPitch(n); }
static void density_setRndPitch(t_density *x, double n)       { x->_granulator->setRndPitch(n); }
static void density_setSamplePosition(t_density *x, double n) { x->_granulator->setSamplePosition(n); }
static void density_setPhase(t_density *x, double n)          { x->_granulator->setPhase(n); }

// "info": manda dall'uscita di destra il numero di grani attivi (outlet_int = un intero da un'uscita)
static void density_info(t_density *x)  { outlet_int(x->m_outlet1, x->_granulator->getNumOscActive()); }
static void density_grain(t_density *x) { x->_granulator->grain(); }

// Il testo che compare passando con il mouse su entrate e uscite.
//   m = ASSIST_INLET o ASSIST_OUTLET     a = quale (0 = la prima a sinistra)
//   s = dove scrivere il testo: una stringa di Max lunga ASSIST_MAX_STRING_LEN
static void density_assist(t_density *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_OUTLET) {
        switch (a) {
            case 0: snprintf(s, ASSIST_MAX_STRING_LEN, "(signal) Output Left Signal"); break;
            case 1: snprintf(s, ASSIST_MAX_STRING_LEN, "(signal) Output Right Signal"); break;
            case 2: snprintf(s, ASSIST_MAX_STRING_LEN, "(message) Output Granulator Informations"); break;
        }
    }
    else
        snprintf(s, ASSIST_MAX_STRING_LEN, "(message) Configure Granulator");
}


// ============================================================================================
//  4. NASCITA E MORTE DELL'OGGETTO
// ============================================================================================

// Max la chiama ogni volta che nella patch si scrive [ape.density~ ...].
// La classe e' dichiarata A_GIMME: gli argomenti scritti dopo il nome arrivano in ac / av.
// Ritorna il nuovo oggetto, oppure NULL (Max allora mostra l'oggetto come "non creato").
static void *density_new(t_symbol *s, long ac, t_atom *av)
{
    // Memoria per UNA struttura t_density, gia' collegata alla classe. Mai malloc o new qui:
    // l'oggetto appartiene a Max, che lo distrugge con object_free.
    t_density *x = (t_density *) object_alloc(density_class);
    if (!x)
        return NULL;

    // object_alloc non chiama costruttori C++: i campi vanno riempiti a mano.
    x->samplebuf = x->windowbuf = NULL;
    x->samplename = x->windowname = NULL;
    x->_granulator = new apeDensity(MAXGRAINS, sys_getsr());   // sys_getsr: la frequenza attuale di Max

    // Prepara la parte audio dell'oggetto e crea le ENTRATE di segnale: qui 1.
    // Da quell'entrata non leggiamo audio: in Max l'entrata di sinistra riceve anche i messaggi.
    dsp_setup((t_pxobject *)x, 1);

    // Le USCITE si creano da destra a sinistra: la prima creata e' quella piu' a destra.
    //   intout             un'uscita per numeri interi (ci serve il suo indirizzo, per outlet_int)
    //   outlet_new "signal" un'uscita di segnale: in perform64 diventa outs[0], outs[1] ...
    x->m_outlet1 = intout((t_object *)x);
    outlet_new((t_object *)x, "signal");
    outlet_new((t_object *)x, "signal");

    density_setbuf(x, s, ac, av);                   // argomenti: [ape.density~ file finestra]
    return x;
}

// Max la chiama quando l'oggetto viene cancellato (o la patch chiusa). Qui si libera tutto
// quello che density_new ha creato, in ordine inverso. La struttura la libera Max.
static void density_free(t_density *x)
{
    // Per prima cosa: stacca l'oggetto dalla catena audio. Dopo questa riga perform64
    // non viene piu' chiamata, e si puo' distruggere il resto senza pericolo.
    dsp_free((t_pxobject *)x);

    if (x->samplebuf) object_free(x->samplebuf);    // i riferimenti sono oggetti di Max: object_free
    if (x->windowbuf) object_free(x->windowbuf);
    delete x->_granulator;                          // il nucleo e' C++: delete
}


// ============================================================================================
//  5. LA CLASSE: ext_main
// ============================================================================================

// Il punto d'ingresso dell'external: Max lo chiama una volta, quando carica il file .mxo,
// cioe' la prima volta che serve un oggetto con questo nome.
// E' dichiarata in ext.h come  void C74_EXPORT ext_main(void *r):  C74_EXPORT la rende visibile
// da fuori della libreria; tutto il resto di questo file resta nascosto.
// Nelle versioni vecchie dell'SDK questa funzione si chiamava "int main(void)".
void ext_main(void *r)
{
    // class_new crea la classe:
    //   "ape.density~"   il nome che si scrive nella patch (uguale al nome del file .mxo)
    //   density_new      la funzione che crea un oggetto
    //   density_free     la funzione che lo distrugge
    //   sizeof(...)      quanta memoria serve per un oggetto
    //   0L               non usato (una volta: la funzione per il menu)
    //   A_GIMME, 0       il tipo degli argomenti dell'oggetto; lo 0 chiude l'elenco
    // "(method)" e' una conversione: per Max tutte le funzioni sono indirizzi dello stesso tipo.
    // [2026] A_GIMME da solo (prima: A_GIMME, A_DEFLONG)
    t_class *c = class_new("ape.density~", (method)density_new, (method)density_free, sizeof(t_density), 0L, A_GIMME, 0);

    // class_addmethod lega una PAROLA a una FUNZIONE: quando all'oggetto arriva quel messaggio,
    // Max chiama quella funzione. Dopo la parola ci sono i tipi degli argomenti, chiusi da 0.
    //
    // I messaggi "di sistema": li manda Max, non l'utente. A_CANT = "non si puo' mandare da una patch".
    class_addmethod(c, (method)density_dsp64, "dsp64", A_CANT, 0);        // si accende l'audio
    class_addmethod(c, (method)density_notify, "notify", A_CANT, 0);      // avvisi (qui: dai buffer~)
    class_addmethod(c, (method)density_assist, "assist", A_CANT, 0);      // testo su entrate e uscite
    class_addmethod(c, (method)density_dblclick, "dblclick", A_CANT, 0);  // doppio clic sull'oggetto

    // I messaggi dell'utente. Max controlla e converte gli argomenti prima di chiamarci:
    //   A_LONG un intero    A_FLOAT un decimale    A_SYM una parola    A_GIMME tutto, senza controllo
    class_addmethod(c, (method)density_start, "start", A_LONG, 0);
    class_addmethod(c, (method)density_setSample, "setSample", A_SYM, 0);
    class_addmethod(c, (method)density_setWindowing, "setWindowing", A_SYM, 0);
    class_addmethod(c, (method)density_setbuf, "setbuf", A_GIMME, 0);

    class_addmethod(c, (method)density_interp, "interp", A_LONG, 0);
    class_addmethod(c, (method)density_overlap, "overlap", A_LONG, 0);
    class_addmethod(c, (method)density_reverse, "reverse", A_LONG, 0);

    class_addmethod(c, (method)density_setLevel, "setLevel", A_FLOAT, 0);
    class_addmethod(c, (method)density_setWidth, "setWidth", A_FLOAT, 0);
    class_addmethod(c, (method)density_setPanningMode, "setPanningMode", A_LONG, 0);

    class_addmethod(c, (method)density_setLength, "setLength", A_FLOAT, 0);
    class_addmethod(c, (method)density_setRndLength, "setRndLength", A_FLOAT, 0);
    class_addmethod(c, (method)density_setDensity, "setDensity", A_FLOAT, 0);
    class_addmethod(c, (method)density_setRndDensity, "setRndDensity", A_FLOAT, 0);
    class_addmethod(c, (method)density_setPitch, "setPitch", A_FLOAT, 0);
    class_addmethod(c, (method)density_setRndPitch, "setRndPitch", A_FLOAT, 0);
    class_addmethod(c, (method)density_setSamplePosition, "setSamplePosition", A_FLOAT, 0);
    class_addmethod(c, (method)density_setPhase, "setPhase", A_FLOAT, 0);

    class_addmethod(c, (method)density_info, "info", 0);                  // senza argomenti
    class_addmethod(c, (method)density_grain, "grain", 0);

    class_dspinit(c);                  // "questa classe fa audio": aggiunge i messaggi comuni a tutti gli oggetti MSP
    class_register(CLASS_BOX, c);      // la classe diventa disponibile. CLASS_BOX = oggetti che si mettono in una patch
    density_class = c;                 // la teniamo da parte: serve a density_new (object_alloc)

    post(APE_MSG);                                   // [2026] una volta sola, non a ogni oggetto creato
}
