#include "mapper.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    MapperConfig config;
    MapperState state;
    MapperInput input;
    MapperReport output;

    mapper_default_config(&config);
    mapper_reset(&state);
    memset(&input, 0, sizeof(input));

    input.keys['w'] = true;
    input.keys['d'] = true;
    input.keys[MAP_KEY_SPACE] = true;
    input.mouse_left = true;
    input.mouse_dx = 10;
    input.mouse_dy = -5;

    mapper_step(&state, &config, &input, &output);

    assert(output.left_x == 127);
    assert(output.left_y == -127);
    assert((output.buttons & MAP_BTN_CROSS) != 0);
    assert((output.triggers & MAP_TRIGGER_R2) != 0);
    assert(output.right_x == 20);
    assert(output.right_y == -10);

    memset(&input, 0, sizeof(input));
    mapper_step(&state, &config, &input, &output);
    assert(output.left_x == 0);
    assert(output.left_y == 0);
    assert(output.right_x == 14); /* 20 * 0.72, truncated */
    assert(output.right_y == -7);

    memset(&input, 0, sizeof(input));
    input.keys['a'] = true;
    input.keys['s'] = true;
    input.mouse_right = true;
    input.mouse_middle = true;
    mapper_step(&state, &config, &input, &output);
    assert(output.left_x == -127);
    assert(output.left_y == 127);
    assert((output.triggers & MAP_TRIGGER_L2) != 0);
    assert((output.buttons & MAP_BTN_R3) != 0);

    config.invert_mouse_y = true;
    mapper_reset(&state);
    memset(&input, 0, sizeof(input));
    input.mouse_dy = 10;
    mapper_step(&state, &config, &input, &output);
    assert(output.right_y == -20);

    puts("mapper tests: PASS");
    return 0;
}
