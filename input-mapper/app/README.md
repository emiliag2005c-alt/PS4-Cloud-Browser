# PS4 Input Mapper — standalone editor app

This directory contains an isolated OpenOrbis application target. It does not alter the Cloud Browser target at the repository root.

## Current state

The first app shell reuses the repository's existing VideoOut framebuffer renderer and presents the intended PC-default bindings. It is a **renderer/package scaffold**, not yet an interactive editor.

Not implemented or verified yet:
- keyboard/gamepad navigation inside this app;
- live keyboard or mouse capture;
- editing controls in the UI;
- profile serialization and save/load on PS4;
- virtual DualShock output or game-input injection.

The source intentionally leaves keyboard and mouse startup disabled. Earlier experiments in the Cloud Browser project showed instability after input startup, and the mouse ABI declarations there are not verified. This app must not enable those paths until they are independently tested.

## Build

The dedicated workflow is `.github/workflows/build-input-mapper.yml`. It uses OpenOrbis Toolchain v0.5.4 and runs:

```sh
make -C input-mapper/app
```

The expected package is:

```
IV0000-PIM000001_00-PS4INPUTMAPPER0.pkg
```

The package uses title ID `PIM000001`, separate from the Cloud Browser's `PSCB00001`.

## Next engineering steps

1. Connect `../src/profile_model.c` to the app and replace the hardcoded rows with model data.
2. Add safe profile persistence and host-side tests.
3. Implement navigation only after selecting and validating a PS4 input path.
4. Verify keyboard and mouse separately on hardware.
5. Investigate runtime gamepad injection as a separate GoldHEN component; a normal PKG cannot be assumed to inject a virtual controller into arbitrary games.
