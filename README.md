# ApeExternals

Two Max/MSP objects by Alessandro Petrolati ([apeSoft](https://www.apesoft.it)).

- **ape.density~** — granular synthesis on a sound file.
- **ape.pulsaret~** — pulsar synthesis with morphing between two waveforms.

Version 1.2 (2026). Built with the current Max SDK ([max-sdk-base](https://github.com/Cycling74/max-sdk-base), API 8.2).

## Compatibility

| | Works with |
|---|---|
| **Max** | Max 8.2 or later, Max 9 |
| **macOS** | macOS 12 or later, Apple silicon and 64-bit Intel (universal objects) |
| **Windows** | 64-bit, built by GitHub Actions — **not tested yet in Max for Windows** |

## Install

The objects are distributed as a Max package: a folder named `ApeExternals` to copy into
`Documents/Max 9/Packages` (or `Documents/Max 8/Packages`). See [help/README.md](help/README.md).

## How the code is organised

    source/
      nucleo/            the engines: plain C++17, no Max headers, usable outside Max
        apeGrain.h           one grain: the two inner loops
        apeGranulator.*      what the two engines share: voices, scheduler, random numbers
        apeDensity.*         the engine of ape.density~
        apePulsaret.*        the engine of ape.pulsaret~
      ape.density~/      the Max wrapper of the first object (class, messages, buffer~, audio)
      ape.pulsaret~/     the Max wrapper of the second object
      Info.plist         the bundle identity (macOS)
    help/                the two help patchers and the package README
    prova/               a command-line test of the engines, without Max
    max-sdk-base/        the Max SDK (downloaded when you build; not in the repository)

The comments in the source are in Italian: `ape.density~.cpp` is written as a step-by-step guide
to the Max SDK ("nucleo" = engine, "involucro" = wrapper).

## Build

    git clone https://github.com/alessandropetrolati/ApeExternals.git

The Max SDK is not in the repository. `costruisci.command` and CMake download it by themselves
(a fixed version of max-sdk-base). For the Xcode project alone, get it once with:

    git clone https://github.com/Cycling74/max-sdk-base.git max-sdk-base

**macOS, Xcode project.** Open `ApeExternals.xcodeproj` and build the two targets in Release,
or double-click `costruisci.command`: it builds both objects, makes the Max package, zips it
into `distribuzione/` and installs it into `Documents/Max 8/Packages` and `Max 9/Packages`.

**macOS or Windows, CMake** (3.25 or later):

    cmake -B build                  # on macOS add: -G Xcode
    cmake --build build --config Release

The objects are written into `externals/`.

**Windows without a Windows machine.** Every push is built by GitHub Actions
(see `.github/workflows/build.yml`). Open the *Actions* tab, choose the latest run and download
the Max package from *Artifacts*: `ApeExternals-<version>-Windows` or `ApeExternals-<version>-macOS`.
Each one contains the `ApeExternals` folder, ready to copy into `Packages`.

**Test of the engines, without Max:**

    cd prova
    c++ -std=c++17 -O3 -I../source/nucleo prova_nucleo.cpp ../source/nucleo/*.cpp -o prova_nucleo && ./prova_nucleo

## Copyright

Copyright (c) 2026 Alessandro Petrolati. All rights reserved.
The Max SDK in `max-sdk-base` is by Cycling '74, under its own licence.
