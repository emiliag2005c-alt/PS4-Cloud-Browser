#include "mapper.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

static int8_t clamp_axis(float value)
{
    if (value > 127.0f) value = 127.0f;
    if (value < -127.0f) value = -127.0f;
    return (int8_t)value;
}

static int8_t digital_axis(bool positive, bool negative)
{
    if (positive == negative) return 0;
    return positive ? 127 : -127;
}

void mapper_default_config(MapperConfig *config)
{
    if (config == NULL) return;
    config->mouse_sensitivity_x = 2.0f;
    config->mouse_sensitivity_y = 2.0f;
    config->aim_decay = 0.72f;
    config->invert_mouse_y = false;
}

void mapper_reset(MapperState *state)
{
    if (state == NULL) return;
    state->aim_x = 0.0f;
    state->aim_y = 0.0f;
}

void mapper_step_profile(MapperState *state,
                         const MapperConfig *config,
                         const MapperInput *input,
                         MapperReport *output,
                         enum MapperProfile profile)
{
    float dx;
    float dy;

    if (state == NULL || config == NULL || input == NULL || output == NULL)
        return;

    memset(output, 0, sizeof(*output));

    output->left_x = digital_axis(input->keys['D'] || input->keys['d'],
                                  input->keys['A'] || input->keys['a']);
    output->left_y = digital_axis(input->keys['S'] || input->keys['s'],
                                  input->keys['W'] || input->keys['w']);

    dx = (float)input->mouse_dx * config->mouse_sensitivity_x;
    dy = (float)input->mouse_dy * config->mouse_sensitivity_y;
    if (config->invert_mouse_y) dy = -dy;

    state->aim_x = state->aim_x * config->aim_decay + dx;
    state->aim_y = state->aim_y * config->aim_decay + dy;
    state->aim_x = (float)clamp_axis(state->aim_x);
    state->aim_y = (float)clamp_axis(state->aim_y);

    output->right_x = clamp_axis(state->aim_x);
    output->right_y = clamp_axis(state->aim_y);

    if (input->keys[MAP_KEY_SPACE]) output->buttons |= MAP_BTN_CROSS;
    if (input->keys['F'] || input->keys['f']) output->buttons |= MAP_BTN_CIRCLE;
    if (input->keys['R'] || input->keys['r']) output->buttons |= MAP_BTN_SQUARE;
    if (input->keys['E'] || input->keys['e']) output->buttons |= MAP_BTN_TRIANGLE;
    if (input->keys['Q'] || input->keys['q']) output->buttons |= MAP_BTN_L1;
    if (input->keys[MAP_KEY_SHIFT]) output->buttons |= MAP_BTN_L3;

    if (input->mouse_middle) output->buttons |= MAP_BTN_R3;

    if (profile == MAP_PROFILE_GOW_RAGNAROK) {
        /* GoW Ragnarök default: left click = light attack (R1),
           right click = heavy/charged attack (R2). Held input stays active. */
        if (input->mouse_left) output->buttons |= MAP_BTN_R1;
        if (input->mouse_right) output->triggers |= MAP_TRIGGER_R2;
    } else {
        if (input->mouse_right) output->triggers |= MAP_TRIGGER_L2;
        if (input->mouse_left) output->triggers |= MAP_TRIGGER_R2;
    }
}


void mapper_step(MapperState *state,
                 const MapperConfig *config,
                 const MapperInput *input,
                 MapperReport *output)
{
    mapper_step_profile(state, config, input, output, MAP_PROFILE_DEFAULT);
}
