# PS4 Cloud Browser

An experimental homebrew browser project for PlayStation 4 (CUH-1215A with GoldHEN).

## Goal
Build and validate a minimal, original PS4 application first, then investigate practical browser-engine options. GeForce NOW support is an experimental long-term target, not a guaranteed feature.

## Roadmap
- [ ] 0.1 — Establish a reproducible build and produce a test PKG.
- [ ] 0.2 — Add a native interface and controller / keyboard / mouse input.
- [ ] 0.3 — Evaluate a feasible web-rendering engine for PS4.
- [ ] 0.4 — Investigate GeForce NOW compatibility and its video / input requirements.

## Important limitations
A successful PKG build does not prove that the application can launch on every PS4 firmware. Modern websites and cloud-gaming services may require browser features, codecs, memory, and DRM support that a homebrew app may not provide.

## Build
The project is intended to use GitHub Actions and the OpenOrbis PS4 toolchain. Build instructions will be added once the initial workflow is validated.

## Safety
Do not install experimental PKG files until their build artifacts and contents have been reviewed.