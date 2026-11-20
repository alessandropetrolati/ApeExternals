# ApeExternals — ape.density~ and ape.pulsaret~ for Max

Two Max/MSP objects by Alessandro Petrolati (apeSoft, www.apesoft.it).

- **ape.density~**: granular synthesis on a sound file.
- **ape.pulsaret~**: pulsar synthesis with morphing between two waveforms.

Version 1.2 (2026).

## Compatibility

| | Works with | Does not work with |
|---|---|---|
| **Processor** | Apple silicon (M1, M2, M3, M4 and later) and 64-bit Intel. The objects are "universal": one file for both kinds of Mac | 32-bit Macs, PowerPC |
| **System** | macOS 12 Monterey or later. Windows 64-bit, with the Windows package (it contains the `.mxe64` files): **not tested yet in Max for Windows** | macOS 11 or earlier; 32-bit Windows |
| **Max** | Max 8.2 or later, Max 9 | Max 8.1 or earlier, Max 7 |

Notes:
- On a Mac with Apple silicon the objects run natively. If Max runs under Rosetta, they use the Intel part of the same file.
- Max for Live: the objects are ordinary Max externals; use inside Ableton Live has not been tested.

## Installation

This folder, `ApeExternals`, is a **Max package**: copy the whole folder, without opening it.

1. Quit Max.
2. Open the folder `Documents/Max 9/Packages` (for Max 8: `Documents/Max 8/Packages`).
   On Windows the folder is the same, inside your `Documents`.
   If the `Packages` folder is not there, create it.
3. Copy the whole `ApeExternals` folder into it.
4. If you have an older version of the same objects, remove it: Max must find only one.
5. Open Max.

## Try it

1. In a new patcher type `ape.density~`.
2. The Max Console shows the line `ape.density~ 1.2 by Alessandro Petrolati …`.
3. Hold Option (⌥) and click the object: its help file opens.
4. Turn the audio on and click the first toggle at the top left (`start`).

Do the same with `ape.pulsaret~`.

## If Max does not load the object

**"ape.density~: no such object"**
The folder is not in the right place. Check that this file exists:
`Documents/Max 9/Packages/ApeExternals/externals/ape.density~.mxo`.

**"cannot be loaded due to system security policy", or "is damaged"**
This happens when the package arrives from the Internet or by e-mail: macOS puts it in quarantine.
Open Terminal and type (for Max 8 change the number):

    xattr -dr com.apple.quarantine ~/Documents/Max\ 9/Packages/ApeExternals

Then quit Max and open it again.

## What is in the folder

    ApeExternals/
      externals/           the two objects: .mxo for macOS (and .mxe64 for Windows, when present)
      help/                the two help files (.maxhelp)
      package-info.json    the description of the package, for Max
      README.md            this file

## Uninstall

Quit Max and delete the `ApeExternals` folder from `Packages`.
