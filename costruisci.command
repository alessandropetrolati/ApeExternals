#!/bin/bash
# Doppio clic nel Finder: compila ape.density~ e ape.pulsaret~ (in externals/), prepara il pacchetto di Max,
# ne fa uno zip (in distribuzione/) e lo installa in Max 8 e Max 9.
cd "$(dirname "$0")" || exit 1
# Usa il progetto Xcode che sta in questa cartella, ApeExternals.xcodeproj: lo stesso che si apre
# con il doppio clic. Qui Xcode lavora "da riga di comando" (xcodebuild), senza aprire la finestra.
#   -alltargets            tutti e due gli oggetti
#   -configuration Release la versione veloce, per Apple silicon e Intel insieme
#   OBJROOT, SYMROOT       i file di lavoro vanno in una cartella temporanea, non qui dentro
# L'SDK di Max non sta nella repository: se manca, lo si scarica qui accanto (una volta sola).
# Il progetto Xcode lo cerca in max-sdk-base/c74support.
if [ ! -d max-sdk-base/c74support ]; then
    echo "== Scarico l'SDK di Max (max-sdk-base) da GitHub"
    git clone https://github.com/Cycling74/max-sdk-base.git max-sdk-base \
        && git -C max-sdk-base checkout -q c03a2922a2a8ff149165a1e2ea134e9321d6e202 \
        || { read -r -p "Non riesco a scaricare l'SDK. Invio per chiudere"; exit 1; }
fi
if ! command -v xcodebuild >/dev/null 2>&1; then
    echo "xcodebuild non trovato: serve Xcode."; read -r -p "Invio per chiudere"; exit 1
fi
lavoro="${TMPDIR:-/tmp}/ApeExternals.build"
echo "== $(xcodebuild -version | head -1)"
xcodebuild -project ApeExternals.xcodeproj -alltargets -configuration Release \
           OBJROOT="$lavoro/obj" SYMROOT="$lavoro/sym" build \
    || { read -r -p "Errore nella compilazione. Invio per chiudere"; exit 1; }
echo
echo "== Compilati:"
for n in "ape.density~" "ape.pulsaret~"; do
    printf "%s: " "$n"; lipo -archs "externals/$n.mxo/Contents/MacOS/$n" 2>/dev/null || { echo "non compilato"; read -r -p "Invio per chiudere"; exit 1; }
done

# IL PACCHETTO. Un pacchetto di Max e' una cartella con dentro sottocartelle dal nome fisso:
#   ApeExternals/externals          i due .mxo
#   ApeExternals/help               i due .maxhelp
#   ApeExternals/package-info.json  nome, versione, autore
#   ApeExternals/README.md          la guida all'installazione (e' help/README.md)
# Max legge da solo tutte le cartelle dentro Documenti/Max 8/Packages: non serve File Preferences.
# Lo prepariamo una volta, in una cartella temporanea, e lo usiamo due volte: zip e installazione.
versione=$(sed -n 's/.*"version" *: *"\([^"]*\)".*/\1/p' package-info.json | head -1)
pacchetto="$lavoro/pacchetto/ApeExternals"
rm -rf "$lavoro/pacchetto"
mkdir -p "$pacchetto/externals" "$pacchetto/help"
ditto "externals/ape.density~.mxo"  "$pacchetto/externals/ape.density~.mxo"     # ditto: copia un bundle senza rovinarne la firma
ditto "externals/ape.pulsaret~.mxo" "$pacchetto/externals/ape.pulsaret~.mxo"
cp "help/ape.density~.maxhelp" "help/ape.pulsaret~.maxhelp" "$pacchetto/help/"
cp package-info.json "$pacchetto/"
cp help/README.md "$pacchetto/README.md"

# 1) Lo ZIP da dare ad altri: distribuzione/ApeExternals-<versione>-macOS.zip
#    Dentro c'e' la cartella ApeExternals, pronta da copiare in Packages.
mkdir -p distribuzione
zip="distribuzione/ApeExternals-${versione:-1.1.0}-macOS.zip"
rm -f "$zip"
ditto -c -k --keepParent "$pacchetto" "$zip" || { read -r -p "Errore nella creazione dello zip. Invio per chiudere"; exit 1; }
echo
echo "== Zip: $zip ($(du -h "$zip" | cut -f1 | tr -d ' '))"

# 2) L'installazione su questo Mac, in ogni versione di Max che ha gia' la sua cartella in Documenti.
#    La copia vecchia del pacchetto viene sostituita.
echo
echo "== Installazione:"
installati=0
for max in "Max 8" "Max 9"; do
    [ -d "$HOME/Documents/$max" ] || continue
    destinazione="$HOME/Documents/$max/Packages/ApeExternals"
    rm -rf "$destinazione"
    mkdir -p "$HOME/Documents/$max/Packages" && ditto "$pacchetto" "$destinazione" || { echo "non posso scrivere in $destinazione"; continue; }
    echo "$max: $destinazione"
    installati=$((installati + 1))
done
[ "$installati" -eq 0 ] && echo "Nessuna cartella Documenti/Max 8 o Documenti/Max 9 trovata: niente da installare."

echo
echo "Chiudi e riapri Max, poi scrivi ape.density~ in una patch (aiuto: Opzione-clic sull'oggetto)."
read -r -p "Invio per chiudere"
