# PS4 Cloud Browser

Experimental homebrew application for PlayStation 4, initially targeting a CUH-1215A running GoldHEN.

## Current version: 0.3 graphical interface prototype

Version 0.2 was confirmed to launch and remain running on the target PS4. Version 0.3 adds a first static 1080p interface rendered into frame buffers through OpenOrbis' VideoOut API.

The interface is a graphics milestone only. It does **not** include a web engine, network browser, controller navigation, or GeForce NOW support yet.

## Build

Every push to `main`, or a manual run from the Actions tab, starts the **Build PS4 Cloud Browser** workflow.

The workflow downloads OpenOrbis PS4 Toolchain v0.5.4, compiles the application, creates the fake SELF `eboot.bin`, generates package metadata and an icon, packages the files into a PS4 `.pkg`, validates it, and uploads the build artifacts.

## Roadmap

- [x] Create the repository and initial build structure.
- [x] 0.2 — Confirm that the application launches and remains running on the target PS4.
- [ ] 0.3 — Render the first native graphical interface.
- [ ] 0.4 — Add DualShock 4 navigation and basic input.
- [ ] 0.5 — Research and prototype a feasible web-rendering engine.
- [ ] 0.6 — Investigate GeForce NOW compatibility, including networking, video decoding, and input latency.

## Important limitations

A successful build only proves that the toolchain produced a package. The 0.2 boot test was confirmed on one PS4; the 0.3 graphics prototype still needs console testing.

Modern websites and cloud-gaming services may require browser APIs, codecs, memory, and DRM support that a homebrew application may not provide. GeForce NOW compatibility is not guaranteed.

Do not install an experimental PKG until its build has succeeded and the artifact has been reviewed.
