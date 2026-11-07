# PS4 Cloud Browser

Experimental homebrew application for PlayStation 4, initially targeting a CUH-1215A running GoldHEN.

## Current version: 0.4 video diagnostics

Version 0.2 was confirmed to launch and remain running on the target PS4. Version 0.3 added a first static 1080p interface, but the console test remained on a black screen.

Version 0.4 replaces the custom framebuffer setup with an implementation following the OpenOrbis graphics sample pattern more closely. It records initialization and frame-flip stages to `/data/PS4CloudBrowser.log` and displays a diagnostic interface if video initialization succeeds.

This is still a graphics diagnostic only. It does **not** include a web engine, network browser, controller navigation, or GeForce NOW support.

## Build

Every push to `main`, or a manual run from the Actions tab, starts the **Build PS4 Cloud Browser** workflow.

The workflow downloads OpenOrbis PS4 Toolchain v0.5.4, compiles the application, creates the fake SELF `eboot.bin`, generates package metadata and an icon, packages the files into a PS4 `.pkg`, validates it, and uploads the build artifacts.

## Roadmap

- [x] Create the repository and initial build structure.
- [x] 0.2 — Confirm that the application launches and remains running on the target PS4.
- [x] 0.3 — Build the first native graphical interface (console test showed a black screen).
- [ ] 0.4 — Diagnose VideoOut/framebuffer initialization on the target console.
- [ ] 0.5 — Add DualShock 4 navigation and basic input.
- [ ] 0.6 — Research and prototype a feasible web-rendering engine.
- [ ] 0.7 — Investigate GeForce NOW compatibility, including networking, video decoding, and input latency.

## Important limitations

A successful build only proves that the toolchain produced a package. The 0.2 boot test was confirmed on one PS4; the 0.3 graphics prototype did not display its interface on that console. Version 0.4 must be tested on hardware before its graphics path can be considered working.

Modern websites and cloud-gaming services may require browser APIs, codecs, memory, and DRM support that a homebrew application may not provide. GeForce NOW compatibility is not guaranteed.

Do not install an experimental PKG until its build has succeeded and the artifact has been reviewed.
