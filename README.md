# PS4 Cloud Browser

Experimental homebrew application for PlayStation 4, initially targeting a CUH-1215A running GoldHEN.

## Current version: 0.1 boot-test

The first milestone is a minimal native PS4 application and a repeatable GitHub Actions build. The current program is deliberately a boot-test stub: it starts and sleeps in a loop. It does **not** display a browser interface, render web pages, or connect to GeForce NOW yet.

## Build

Every push to `main`, or a manual run from the Actions tab, starts the **Build PS4 Cloud Browser** workflow.

The workflow:
1. Downloads the OpenOrbis PS4 Toolchain v0.5.4.
2. Compiles the application for the PS4 target.
3. Creates the fake SELF `eboot.bin`.
4. Generates the application icon and SFO metadata.
5. Packages the files into a PS4 `.pkg`.
6. Uploads the PKG and intermediate ELF as workflow artifacts.

## Roadmap

- [x] Create the repository and initial build structure.
- [ ] 0.1 — Validate the toolchain, build workflow, and PKG output.
- [ ] 0.2 — Add a visible native interface and controller / keyboard / mouse input.
- [ ] 0.3 — Research and prototype a feasible web-rendering engine.
- [ ] 0.4 — Investigate GeForce NOW compatibility, including networking, video decoding, and input latency.

## Important limitations

A successful build only proves that the toolchain produced a package. It does not prove that the package launches on every firmware or that it is compatible with a particular PS4. Modern websites and cloud-gaming services may require browser APIs, codecs, memory, and DRM support that a homebrew application may not provide.

Do not install an experimental PKG until its build has succeeded and the artifact has been reviewed.
