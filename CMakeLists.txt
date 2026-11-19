# ape.density~ — external per Max/MSP, compilato con l'SDK di Max attuale (max-sdk-base)
# Alessandro Petrolati, www.apesoft.it
#
# Serve: Xcode, CMake 3.25 o piu' recente (cmake --version; si installa con "brew install cmake").
#
#   cmake -G Xcode -B build          crea build/ApeExternals.xcodeproj
#   cmake --build build --config Release
#
# Risultato: externals/ape.density~.mxo (arm64 + x86_64).
# E' la strada alternativa: costruisci.command usa invece ApeExternals.xcodeproj.

cmake_minimum_required(VERSION 3.25)

# un solo file per i Mac con Apple silicon e con Intel
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture")

# Il sistema piu' vecchio su cui gli external devono funzionare. Va detto PRIMA dell'SDK:
# max-sdk-base, da solo, chiede macOS 10.11, e Xcode 27 lo rifiuta (accetta da 12.0 in su).
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Sistema minimo" FORCE)

# L'SDK di Max (max-sdk-base, di Cycling '74): intestazioni (ext.h, z_dsp.h...), librerie e script di CMake.
# - Se in questa cartella c'e' gia' "max-sdk-base" (lo scarica costruisci.command), si usa quella.
# - Altrimenti CMake lo scarica da GitHub in build/_deps (succede sulle macchine di GitHub Actions).
# La versione e' fissata: e' quella con cui il codice e' stato scritto e provato.
set(APE_MAX_SDK_BASE_COMMIT c03a2922a2a8ff149165a1e2ea134e9321d6e202)      # 17.06.2026
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/max-sdk-base/CMakeLists.txt")
    add_subdirectory(max-sdk-base)
else()
    include(FetchContent)
    FetchContent_Declare(max-sdk-base
        GIT_REPOSITORY https://github.com/Cycling74/max-sdk-base.git
        GIT_TAG        ${APE_MAX_SDK_BASE_COMMIT})
    FetchContent_MakeAvailable(max-sdk-base)
endif()

project(ApeExternals)

# Impostazioni di velocita', uguali per tutti gli external (solo nella versione Release):
# -O3, ottimizzazione su tutto il programma (LTO) e, per la parte Apple Silicon del file
# universale, istruzioni scelte per i processori Apple (M1 e successivi).
function(ape_massima_velocita target)
    set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    if(APPLE)
        target_compile_options(${target} PRIVATE "$<$<CONFIG:Release>:-O3;-fno-math-errno>" -Xarch_arm64 -mcpu=apple-m1)
    elseif(MSVC)
        target_compile_options(${target} PRIVATE /utf-8)     # Visual Studio: i sorgenti sono scritti in UTF-8
    else()
        target_compile_options(${target} PRIVATE "$<$<CONFIG:Release>:-O3;-fno-math-errno>")
    endif()
endfunction()

# un external = una cartella dentro source/
add_subdirectory("source/ape.density~")
add_subdirectory("source/ape.pulsaret~")
