#ifndef PS4_INPUT_MAPPER_H
#define PS4_INPUT_MAPPER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Platform-neutral logical button flags; not a raw ScePad ABI. */
enum MapperButton {
    MAP_BTN_CROSS    = 1u << 0,
    MAP_BTN_CIRCLE   = 1u << 1,
    MAP_BTN_SQUARE   = 1u << 2,
    MAP_BTN_TRIANGLE = 1u << 3,
    MAP_BTN_L1       = 1u << 4,
    MAP_BTN_R1       = 1u << 5,
    MAP_BTN_L3       = 1u << 6,
    MAP_BTN_R3       = 1u << 7,
    MAP_BTN_OPTIONS  = 1u << 8,
    MAP_BTN_SHARE    = 1u << 9
};

enum MapperProfile {
    MAP_PROFILE_DEFAULT = 0,
    MAP_PROFILE_GOW_RAGNAROK = 1
};

enum MapperTrigger {
    MAP_TRIGGER_L2 = 1u << 0,
    MAP_TRIGGER_R2 = 1u << 1
};

typedef struct MapperInput {
    /* Set to true while the corresponding USB-HID key is held.
       Key values use ASCII for letters and the documented constants below. */
    bool keys[256];
    bool mouse_left;
    bool mouse_right;
    bool mouse_middle;
    int16_t mouse_dx;
    int16_t mouse_dy;
} MapperInput;

enum MapperKey {
    MAP_KEY_SPACE = 32,
    MAP_KEY_SHIFT = 128,
    MAP_KEY_CTRL  = 129
};

/* Normalized output. Axes are signed [-127, 127], triggers [0, 255].
   This is an internal representation, not a native PS4 SDK structure. */
typedef struct MapperReport {
    uint16_t buttons;
    uint8_t triggers;
    int8_t left_x;
    int8_t left_y;
    int8_t right_x;
    int8_t right_y;
} MapperReport;

typedef struct MapperConfig {
    float mouse_sensitivity_x;
    float mouse_sensitivity_y;
    float aim_decay;       /* 0.0 = snap to center, 1.0 = retain aim */
    bool invert_mouse_y;
} MapperConfig;

typedef struct MapperState {
    float aim_x;
    float aim_y;
} MapperState;

void mapper_default_config(MapperConfig *config);
void mapper_reset(MapperState *state);
void mapper_step_profile(MapperState *state,
                         const MapperConfig *config,
                         const MapperInput *input,
                         MapperReport *output,
                         enum MapperProfile profile);
void mapper_step(MapperState *state,
                 const MapperConfig *config,
                 const MapperInput *input,
                 MapperReport *output);

#ifdef __cplusplus
}
#endif

#endif
