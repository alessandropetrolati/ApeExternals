set_property(DIRECTORY PROPERTY max::external YES)
project(ape.pulsaret_tilde)                          # ->  ape.pulsaret~.mxo

target_sources(ape.pulsaret_tilde PRIVATE
    ape.pulsaret~.cpp                # l'involucro
    ../nucleo/apePulsaret.cpp        # il nucleo di questo oggetto
    ../nucleo/apeGranulator.cpp)     # il nucleo comune
target_include_directories(ape.pulsaret_tilde PRIVATE ../nucleo)
ape_massima_velocita(ape.pulsaret_tilde)
