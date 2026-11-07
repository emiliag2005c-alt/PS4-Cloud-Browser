# PS4 Universal Input Mapper — Prototype 0.2

Experimental input-translation core for PS4 homebrew.

## What this prototype does

- Converts a normalized keyboard/mouse state into a **platform-neutral DualShock-style report**.
- WASD controls the left stick.
- Mouse movement controls a decaying right-stick aim value.
- Mouse buttons and keyboard keys map to logical controller buttons/triggers.
- Includes a selectable **God of War Ragnarök** profile: left click = R1 (light attack), right click = R2 (heavy/charged attack).
- Includes a host-side unit test that can be compiled with a normal C compiler.

## What it does not do yet

This is **not an installable PS4 mapper** and does not inject input into games. It deliberately does not call undocumented PS4 input APIs, patch game processes, or pretend that a normal homebrew application can create a system-wide virtual DualShock 4.

The remaining platform work is substantial:

1. Identify a supported way to read USB HID keyboard and mouse reports on the target firmware.
2. Identify a supported, safe input-injection route visible to games.
3. Verify whether GoldHEN plugin facilities can provide the required per-title integration without destabilizing the system.
4. Only then connect this core to PS4-specific adapters and package it.

A universal solution may not be possible for every title. Games with native keyboard/mouse support should continue using their native path; this project targets games that only read a controller.

## Build and test the portable core

From this directory:

```sh
cc -std=c11 -Wall -Wextra -Werror -Iinclude src/mapper.c tests/test_mapper.c -o mapper_test
./mapper_test
```

## Current mapping

| Input | Logical controller output |
|---|---|
| W / A / S / D | Left stick |
| Mouse movement | Right stick / aim |
| Left mouse button | R2 |
| Right mouse button | L2 |
| Middle mouse button | R3 |
| Space | Cross |
| R | Square |
| E | Triangle |
| Q | L1 |
| F | Circle |
| Left Shift | L3 |

The mapping is a starting default, not a claim that every game's controls are identical.

## Safety / stability rule

The mapper core stays separate from the existing Cloud Browser build and source tree. No kernel patching, syscall patching, or undocumented ABI calls are added to the browser app as part of this prototype.


## God of War Ragnarök profile

Call `mapper_step_profile(..., MAP_PROFILE_GOW_RAGNAROK)` to select this profile. The left mouse button holds logical R1; the right mouse button holds logical R2. Releasing the mouse button releases the corresponding logical action. This profile is only a mapping inside the portable core: it still does not read a physical mouse or inject controller input into a running PS4 game.
