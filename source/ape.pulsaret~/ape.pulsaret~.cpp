//
//  ape.pulsaret~.cpp — l'INVOLUCRO: tutto quello che parla con Max
//
//  Created by Alessandro Petrolati on 21/03/2022. Riscritto nel 2026.
//  Copyright 2026 Prof. All rights reserved.
//
//  Il suono lo fa il NUCLEO (source/nucleo/apePulsaret), che non conosce Max.
//  Qui ci sono solo: la classe dell'oggetto, i messaggi, i buffer~, il collegamento all'audio.
//
// ============================================================================================
//  COME SI LEGGE QUESTO FILE
// ============================================================================================
//
//  E' fatto come ape.density~.cpp, che ha la guida completa all'SDK di Max: conviene leggere
//  prima quello. Qui i commenti dicono soprattutto che cosa e' DIVERSO:
//    - tre buffer~ invece di due (onda 1, finestra, onda 2), trattati con un ciclo;
//    - i messaggi propri della sintesi pulsar (setCps, setGlisson, setMorph, setAttack ...);
//    - 1024 voci al massimo invece di 64.
//  Tutto il resto e' uguale, e non per caso: l'involucro di un external MSP ha sempre questa
//  forma. Cambiano il nome della classe, i messaggi e la riga che chiama il nucleo.
//
//  I cinque momenti, nell'ordine in cui sono scritti (Max li usa al contrario, dal fondo):
//    1. AUDIO      pulsaret_perform64, pulsaret_dsp64
//    2. BUFFER~    setWave1, setWindowing, setWave2, setbuf, notify, dblclick
//    3. MESSAGGI   start, setCps, ...
//    4. OGGETTO    pulsaret_new, pulsaret_free
//    5. CLASSE     ext_main
//

// ---- Le intestazioni dell'SDK (cartella max-sdk-base/c74support) -----------------------------
#include "ext.h"            // la base: t_object, t_symbol, t_atom, post(), outlet_int(), gensym() ...
#include "ext_obex.h"       // le classi: class_new, class_addmethod, object_alloc, object_free
#include "z_dsp.h"          // MSP, cioe' l'audio: t_pxobject, dsp_setup, dsp_free, class_dspinit, sys_getsr
#include "ext_buffer.h"     // l'accesso ai buffer~: t_buffer_ref, buffer_locksamples ...

// ---- Il nostro codice -----------------------------------------------------------------------
#include "apePulsaret.h"    // il nucleo: l'unico punto di contatto con il sintetizzatore
#include <cstring>          // memset
#include <cstdio>           // snprintf

#define APE_MSG "ape.pulsaret~ 1.2 by Alessandro Petrolati © 2026 All Rigth Reserved www.apesoft.it"
#define MAXGRAINS (1024)  // quante voci al massimo: sono create una volta sola, alla nascita dell'oggetto

// La CLASSE (una sola, la crea ext_main) e l'OGGETTO (una struttura per ogni copia nella patch).
// Il primo campo della struttura deve essere t_pxobject: e' la parte che Max conosce.
static t_class *pulsaret_class = NULL;

typedef struct _pulsaret {
    t_pxobject    l_obj;        // prima cosa nella struttura: e' un oggetto MSP
    apePulsaret   *_granulator;  // il nucleo
    t_buffer_ref *wavebuf;       // riferimento al buffer~ con la prima forma d'onda
    t_buffer_ref *windowbuf;     // riferimento al buffer~ con la finestra (inviluppo del grano)
    t_buffer_ref *wavebuf_morph; // riferimento al buffer~ con la seconda forma d'onda (morphing)
    t_symbol     *wavename;      // i tre nomi: servono a pulsaret_notify e a pulsaret_dsp64
    t_symbol     *windowname;
    t_symbol     *wavename_morph;
    void         *m_outlet1;    // uscita di destra: numero di grani attivi
} t_pulsaret;


// ============================================================================================
//  1. AUDIO
// ============================================================================================

// Un blocco audio. Gira nel thread audio: niente post(), niente memoria, niente attese.
// outs[0] e outs[1] sono le due uscite di segnale; sampleframes e' la grandezza del blocco.
static void pulsaret_perform64(t_pulsaret *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    // [2026] prima di tutto silenzio su tutte le uscite, quante sono (prima: 4 uscite azzerate, ma ce n'erano 2)
    for (long c = 0; c < numouts; c++)
        memset(outs[c], 0, sizeof(double) * sampleframes);

    if (numouts < 2 || !x->wavebuf || !x->windowbuf || !x->wavebuf_morph)
        return;

    // dai tre riferimenti ai tre buffer~ veri; NULL = quel buffer~ adesso non esiste
    t_buffer_obj *b[3] = { buffer_ref_getobject(x->wavebuf), buffer_ref_getobject(x->windowbuf), buffer_ref_getobject(x->wavebuf_morph) };
    if (!b[0] || !b[1] || !b[2])
        return;                                          // un buffer~ non esiste (ancora)

    // "lock": finche' non li rilasciamo, Max non sposta e non cancella i campioni
    float   *samples[3];
    apeTable table[3];                                   // onda 1, finestra, onda 2
    bool     ok = true;

    // per ognuno: lock, poi i suoi dati nella struttura del nucleo (apeTable).
    // Il lock si prova su tutti e tre anche se uno fallisce: cosi' il rilascio, sotto, e' semplice.
    for (int i = 0; i < 3; i++) {
        samples[i] = buffer_locksamples(b[i]);
        table[i].data     = samples[i];
        table[i].frames   = (long) buffer_getframecount(b[i]);
        table[i].channels = (long) buffer_getchannelcount(b[i]);
        table[i].sr       = buffer_getsamplerate(b[i]);
        if (!samples[i]) ok = false;
    }

    if (ok)                                              // tutta la sintesi e' in questa riga
        x->_granulator->Processing(outs[0], outs[1], sampleframes, table[0], table[1], table[2]);

    for (int i = 0; i < 3; i++)                          // ogni lock riuscito va rilasciato, sempre
        if (samples[i]) buffer_unlocksamples(b[i]);
}

// Max la chiama quando si accende l'audio: qui si conoscono frequenza di campionamento e blocco.
static void pulsaret_dsp64(t_pulsaret *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->_granulator->setAbsoluteSR(samplerate);

    // si cercano di nuovo i tre buffer~ per nome: cosi' l'oggetto trova anche un buffer~ creato dopo di lui
    if (x->wavebuf && x->wavename)             buffer_ref_set(x->wavebuf, x->wavename);
    if (x->windowbuf && x->windowname)         buffer_ref_set(x->windowbuf, x->windowname);
    if (x->wavebuf_morph && x->wavename_morph) buffer_ref_set(x->wavebuf_morph, x->wavename_morph);

    // "aggiungi alla catena audio la mia funzione perform" (un messaggio all'oggetto dsp64)
    object_method(dsp64, gensym("dsp_add64"), x, pulsaret_perform64, 0, NULL);
}


// ============================================================================================
//  2. BUFFER~
// ============================================================================================
//
// Come in ape.density~: non un puntatore al buffer~, ma un RIFERIMENTO legato a un nome.
// Qui i riferimenti sono tre, e la parte uguale sta in una funzione sola, pulsaret_setRef.
// "t_buffer_ref **ref" e "t_symbol **name" sono gli indirizzi dei due campi della struttura da riempire.

static void pulsaret_setRef(t_pulsaret *x, t_buffer_ref **ref, t_symbol **name, t_symbol *s)
{
    *name = s;                                           // il nome si tiene da parte
    if (!*ref) *ref = buffer_ref_new((t_object *)x, s);
    else       buffer_ref_set(*ref, s);
}

static void pulsaret_setWave1(t_pulsaret *x, t_symbol *s)     { pulsaret_setRef(x, &x->wavebuf, &x->wavename, s); }
static void pulsaret_setWindowing(t_pulsaret *x, t_symbol *s) { pulsaret_setRef(x, &x->windowbuf, &x->windowname, s); }
static void pulsaret_setWave2(t_pulsaret *x, t_symbol *s)     { pulsaret_setRef(x, &x->wavebuf_morph, &x->wavename_morph, s); }

// "setbuf onda1 finestra onda2": anche gli argomenti dell'oggetto passano di qui
static void pulsaret_setbuf(t_pulsaret *x, t_symbol *s, long ac, t_atom *av)
{
    if (ac > 0 && atom_gettype(av) == A_SYM)     pulsaret_setWave1(x, atom_getsym(av));
    if (ac > 1 && atom_gettype(av + 1) == A_SYM) pulsaret_setWindowing(x, atom_getsym(av + 1));
    if (ac > 2 && atom_gettype(av + 2) == A_SYM) pulsaret_setWave2(x, atom_getsym(av + 2));
}

// Max avvisa quando succede qualcosa a un buffer~ che stiamo seguendo. L'avviso riguarda UN
// buffer~ e va passato SOLO al suo riferimento: passarlo a tutti e tre li fa guardare tutti
// lo stesso buffer~ (errore corretto il 20.11.2026: le due onde diventavano la finestra).
// Di chi parla l'avviso lo dice il nome di chi lo manda. Spiegazione completa in ape.density~.cpp.
static t_max_err pulsaret_notify(t_pulsaret *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if (!sender)
        return MAX_ERR_NONE;

    t_symbol *name = (t_symbol *) object_method((t_object *)sender, gensym("getname"));

    if (name && name == x->wavename && x->wavebuf)             buffer_ref_notify(x->wavebuf, s, msg, sender, data);
    if (name && name == x->windowname && x->windowbuf)         buffer_ref_notify(x->windowbuf, s, msg, sender, data);
    if (name && name == x->wavename_morph && x->wavebuf_morph) buffer_ref_notify(x->wavebuf_morph, s, msg, sender, data);
    return MAX_ERR_NONE;
}

// doppio clic sull'oggetto: apre i tre buffer~
static void pulsaret_dblclick(t_pulsaret *x)
{
    if (x->wavebuf)       buffer_view(buffer_ref_getobject(x->wavebuf));
    if (x->windowbuf)     buffer_view(buffer_ref_getobject(x->windowbuf));
    if (x->wavebuf_morph) buffer_view(buffer_ref_getobject(x->wavebuf_morph));
}


// ============================================================================================
//  3. MESSAGGI
// ============================================================================================
//
// Una funzione per ogni messaggio; il tipo dell'argomento e' quello dichiarato in ext_main
// (A_LONG -> long, A_FLOAT -> double). Ognuna passa il valore al nucleo e basta.

static void pulsaret_start(t_pulsaret *x, long n)
{
    if (n < 0 || n > 1) object_error((t_object *)x, "start message: 0 = off; 1 = on");
    else                x->_granulator->setToggle((int) n);
}

static void pulsaret_interp(t_pulsaret *x, long n)
{
    if (n < 0 || n > 1) object_error((t_object *)x, "interp message: 0 = no interp; 1 = two point interp.");
    else                x->_granulator->setInterp((int) n);
}

static void pulsaret_setPanningMode(t_pulsaret *x, long n)
{
    if (n < 0 || n > 1) object_error((t_object *)x, "setPanningMode message: 0 = random width (use setWidth) 1 = manual (0 = L 1 = R)");
    else                x->_granulator->setPanningMode((int) n);
}

static void pulsaret_setLevel(t_pulsaret *x, double n)          { x->_granulator->setLevel(n); }
static void pulsaret_setWidth(t_pulsaret *x, double n)          { x->_granulator->setWidth(n); }
static void pulsaret_setLength(t_pulsaret *x, double n)         { x->_granulator->setLength(n); }
static void pulsaret_setRndLength(t_pulsaret *x, double n)      { x->_granulator->setRndLength(n); }
static void pulsaret_setDensity(t_pulsaret *x, double n)        { if (n > 0.00001) x->_granulator->setDensity(n); }
static void pulsaret_setRndDensity(t_pulsaret *x, double n)     { x->_granulator->setRndDensity(n); }
// i messaggi propri della sintesi pulsar
static void pulsaret_setAttack(t_pulsaret *x, double n)     { x->_granulator->setAttack(n); }
static void pulsaret_setDecay(t_pulsaret *x, double n)      { x->_granulator->setDecay(n); }
static void pulsaret_setCps(t_pulsaret *x, double n)        { x->_granulator->setCps(n); }
static void pulsaret_setRndCps(t_pulsaret *x, double n)     { x->_granulator->setRndCps(n); }
static void pulsaret_setGlisson(t_pulsaret *x, double n)    { x->_granulator->setGlisson(n); }
static void pulsaret_setRndGlisson(t_pulsaret *x, double n) { x->_granulator->setRndGlisson(n); }
static void pulsaret_setMorph(t_pulsaret *x, double n)      { x->_granulator->setMorph(n); }
static void pulsaret_setRndMorph(t_pulsaret *x, double n)   { x->_granulator->setRndMorph(n); }

static void pulsaret_info(t_pulsaret *x)  { outlet_int(x->m_outlet1, x->_granulator->getNumOscActive()); }
static void pulsaret_grain(t_pulsaret *x) { x->_granulator->grain(); }

// testo che compare passando con il mouse su entrate e uscite (m = entrata o uscita, a = quale)
static void pulsaret_assist(t_pulsaret *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_OUTLET) {
        switch (a) {
            case 0: snprintf(s, ASSIST_MAX_STRING_LEN, "(signal) Output CH1 Signal"); break;
            case 1: snprintf(s, ASSIST_MAX_STRING_LEN, "(signal) Output CH2 Signal"); break;
            case 2: snprintf(s, ASSIST_MAX_STRING_LEN, "(message) Output Granulator Informations"); break;
        }
    }
    else
        snprintf(s, ASSIST_MAX_STRING_LEN, "(message) Configure Granulator");
}


// ============================================================================================
//  4. NASCITA E MORTE DELL'OGGETTO
// ============================================================================================

// Max la chiama ogni volta che nella patch si scrive [ape.pulsaret~ ...]; ac / av sono gli argomenti.
static void *pulsaret_new(t_symbol *s, long ac, t_atom *av)
{
    t_pulsaret *x = (t_pulsaret *) object_alloc(pulsaret_class);
    if (!x)
        return NULL;

    x->wavebuf = x->windowbuf = x->wavebuf_morph = NULL;
    x->wavename = x->windowname = x->wavename_morph = NULL;
    x->_granulator = new apePulsaret(MAXGRAINS, sys_getsr());   // le 1024 voci nascono qui, non nel thread audio

    dsp_setup((t_pxobject *)x, 1);                  // un'entrata (solo messaggi)

    x->m_outlet1 = intout((t_object *)x);           // le uscite si creano da destra a sinistra
    outlet_new((t_object *)x, "signal");
    outlet_new((t_object *)x, "signal");

    pulsaret_setbuf(x, s, ac, av);                   // argomenti: [ape.pulsaret~ onda1 finestra onda2]
    return x;
}

// Libera quello che pulsaret_new ha creato; la struttura la libera Max.
static void pulsaret_free(t_pulsaret *x)
{
    dsp_free((t_pxobject *)x);                      // per prima cosa: stacca l'oggetto dall'audio

    if (x->wavebuf)       object_free(x->wavebuf);
    if (x->windowbuf)     object_free(x->windowbuf);
    if (x->wavebuf_morph) object_free(x->wavebuf_morph);
    delete x->_granulator;
}

// ============================================================================================
//  5. LA CLASSE: ext_main
// ============================================================================================

// Il punto d'ingresso dell'external: Max lo chiama una volta, quando carica il file .mxo.
void ext_main(void *r)
{
    t_class *c = class_new("ape.pulsaret~", (method)pulsaret_new, (method)pulsaret_free, sizeof(t_pulsaret), 0L, A_GIMME, 0);

    // messaggi di sistema: li manda Max (A_CANT = non si possono mandare da una patch)
    class_addmethod(c, (method)pulsaret_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)pulsaret_notify, "notify", A_CANT, 0);
    class_addmethod(c, (method)pulsaret_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)pulsaret_dblclick, "dblclick", A_CANT, 0);

    // messaggi dell'utente: parola, funzione, tipo degli argomenti
    class_addmethod(c, (method)pulsaret_start, "start", A_LONG, 0);
    class_addmethod(c, (method)pulsaret_setWave1, "setWave1", A_SYM, 0);
    class_addmethod(c, (method)pulsaret_setWave2, "setWave2", A_SYM, 0);
    class_addmethod(c, (method)pulsaret_setWindowing, "setWindowing", A_SYM, 0);
    class_addmethod(c, (method)pulsaret_setbuf, "setbuf", A_GIMME, 0);

    class_addmethod(c, (method)pulsaret_interp, "interp", A_LONG, 0);

    class_addmethod(c, (method)pulsaret_setLevel, "setLevel", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setWidth, "setWidth", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setPanningMode, "setPanningMode", A_LONG, 0);
    class_addmethod(c, (method)pulsaret_setAttack, "setAttack", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setDecay, "setDecay", A_FLOAT, 0);

    class_addmethod(c, (method)pulsaret_setLength, "setLength", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setRndLength, "setRndLength", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setDensity, "setDensity", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setRndDensity, "setRndDensity", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setCps, "setCps", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setRndCps, "setRndCps", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setGlisson, "setGlisson", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setRndGlisson, "setRndGlisson", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setMorph, "setMorph", A_FLOAT, 0);
    class_addmethod(c, (method)pulsaret_setRndMorph, "setRndMorph", A_FLOAT, 0);

    class_addmethod(c, (method)pulsaret_info, "info", 0);
    class_addmethod(c, (method)pulsaret_grain, "grain", 0);

    class_dspinit(c);                  // questa classe fa audio
    class_register(CLASS_BOX, c);      // la classe diventa disponibile nelle patch
    pulsaret_class = c;

    post(APE_MSG);                                   // [2026] una volta sola, non a ogni oggetto creato
}
