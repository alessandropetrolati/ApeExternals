# Un external dell'SDK attuale: i sorgenti sono elencati a mano, perche' il nucleo
# (le classi C++ del granulatore) sta in una cartella a parte, comune ai due oggetti.
set_property(DIRECTORY PROPERTY max::external YES)   # questa cartella e' un external di Max
project(ape.density_tilde)                           # "_tilde" diventa "~"  ->  ape.density~.mxo

target_sources(ape.density_tilde PRIVATE
    ape.density~.cpp                 # l'involucro: tutto quello che parla con Max
    ../nucleo/apeDensity.cpp         # il nucleo di questo oggetto
    ../nucleo/apeGranulator.cpp)     # il nucleo comune ai due oggetti (apeGrain.h e' solo intestazione)
target_include_directories(ape.density_tilde PRIVATE ../nucleo)
ape_massima_velocita(ape.density_tilde)              # definita nel CMakeLists.txt principale
