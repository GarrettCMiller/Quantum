# Quantum

Quantum is a historical C++ particle-simulation project by Garrett Miller. It
started as a 2D point-sprite particle editor and evolved into experiments with
3D particles, configurable emitters and physics-inspired interactions.

The source includes Windows/MFC tooling, Direct3D-era rendering, user
interface and particle-system code, and experiments with gravity and other
forces. These experiments are historical work in progress, not a validated
scientific simulation.

## Source and history

- `Quantum/` contains the later project source and Visual Studio project files.
- `Quantum/res/_Particle.cpp` and `_Particle.h` preserve an earlier 2D
  implementation; these files are not known to be part of the active build.
- `legacy/2008/` preserves the separate 2008 source snapshot, shaders, and one
  saved configuration supplied with a later archive. See its README for the
  limitations of that configuration.

## Build status

This code has not been rebuilt or tested for this archival upload. It relies
on Windows, MFC, Direct3D-era components, and a compatible version of
[GLib](https://github.com/GarrettCMiller/GLib), Garrett's C++ framework. The
matching dependency versions, build settings and asset requirements still
need to be established. Included executables are historical artifacts and
have not been verified against the current source.

Third-party dependencies and any creative assets retain their respective
owners' rights. No new license is assigned by this README.
