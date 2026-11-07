# PS4 Cloud Browser

Experimental homebrew application for PlayStation 4, initially targeting a CUH-1215A running GoldHEN.

## Current version: 0.5 keyboard input laboratory

Version 0.2 was confirmed to launch and remain running on the target PS4. Version 0.3 added a first static 1080p interface, but the console test remained on a black screen.

Version 0.4 was tested on the target console and successfully displayed the diagnostic interface with VideoOut active.

Version 0.5 adds an experimental keyboard test panel: initialization through the OpenOrbis keyboard/user-service interfaces, a text-entry field, a key-event counter, and the last HID key code. mouse: desactivado temporalmente en la compilación segura hasta verificar la ABI de la API; el arranque gráfico y la prueba de teclado quedan separados.nput is also prototyped through the recovered `scemouse: desactivado temporalmente en la compilación segura hasta verificar la ABI de la API; el arranque gráfico y la prueba de teclado quedan separados.nit`, `scemouse: desactivado temporalmente en la compilación segura hasta verificar la ABI de la API; el arranque gráfico y la prueba de teclado quedan separados.n`, and `scemouse: desactivado temporalmente en la compilación segura hasta verificar la ABI de la API; el arranque gráfico y la prueba de teclado quedan separados.natures. The test panel reports connection state, relative movement, and left/right click counts. Both input paths remain experimental until compiled and tested on the console. The application still has no web engine or GeForce NOW support.

## Build

Every push to `main`, or a manual run from the Actions tab, starts the **Build PS4 Cloud Browser** workflow.

The workflow downloads OpenOrbis PS4 Toolchain v0.5.1.4, compiles the application, creates the fake SELF `eboot.bin`, generates package metadata and an icon, packages the files into a PS4 `.pkg`, validates it, and uploads the build artifacts.

## Roadmap

- [x] Create the repository and initial build structure.
- [x] 0.2 — Confirm that the application launches and remains running on the target PS4.
- [x] 0.3 — Build the first native graphical interface (console test showed a black screen).
- [x] 0.4 — Diagnose VideoOut/framebuffer initialization on the target console.
- [ ] 0.5 — Prototype keyboard input and a text-entry test field; prototype mouse: desactivado temporalmente en la compilación segura hasta verificar la ABI de la API; el arranque gráfico y la prueba de teclado quedan separados.nt and left/right click reporting.
- [ ] 0.6 — Research and prototype a feasible web-rendering engine.
- [ ] 0.7 — Investigate GeForce NOW compatibility, including networking, video decoding, and input latency.

## Important limitations

A successful build only proves that the toolchain produced a package. The 0.2 boot test was confirmed on one PS4; the 0.3 graphics prototype did not display its interface on that console. Version 0.4 graphics output was confirmed on the target PS4. Version 0.5 keyboard input must be compiled and tested on hardware; mouse: desactivado temporalmente en la compilación segura hasta verificar la ABI de la API; el arranque gráfico y la prueba de teclado quedan separados.not yet present.

Modern websites and cloud-gaming services may require browser APIs, codecs, memory, and DRM support that a homebrew application may not provide. GeForce NOW compatibility is not guaranteed.

Do not install an experimental PKG until its build has succeeded and the artifact has been reviewed.
