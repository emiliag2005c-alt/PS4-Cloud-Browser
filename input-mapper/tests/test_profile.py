#!/usr/bin/env python3
import json
from pathlib import Path

profile_path = Path("input-mapper/profiles/pc-default.json")
profile = json.loads(profile_path.read_text(encoding="utf-8"))

assert profile["format_version"] == 1
assert profile["profile_id"] == "pc-default"
assert profile["input_mode"] == "keyboard_mouse"

movement = profile["movement"]
assert {key: movement[key]["code"] for key in ("up", "left", "down", "right")} == {
    "up": "W", "left": "A", "down": "S", "right": "D"
}

bindings = {item["action"]: item for item in profile["bindings"]}
expected = {
    "R1": ("mouse", "LEFT"),
    "R2": ("mouse", "RIGHT"),
    "L1": ("keyboard", "Q"),
    "L2": ("keyboard", "CTRL"),
    "CROSS": ("keyboard", "SPACE"),
    "INTERACT": ("keyboard", "E"),
    "L3": ("keyboard", "SHIFT"),
    "R3": ("mouse", "MIDDLE"),
    "OPTIONS": ("keyboard", "ESC"),
}
for action, (device, code) in expected.items():
    item = bindings[action]
    assert (item["device"], item["code"]) == (device, code), action

assert "alternates" not in bindings["L2"], "Default L2 must not assume side mouse buttons"
assert profile["look"]["device"] == "mouse"
print("profile JSON tests: PASS")
