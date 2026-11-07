#include "profile_defaults.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static const ProfileBinding *find(const InputProfile *profile, const char *action)
{
    size_t i;
    for (i = 0; i < profile->binding_count; ++i)
        if (strcmp(profile->bindings[i].action, action) == 0)
            return &profile->bindings[i];
    return NULL;
}

int main(void)
{
    InputProfile profile;
    const ProfileBinding *binding;

    assert(input_mapper_init_default_profile(&profile));
    assert(strcmp(profile.id, "pc-default") == 0);
    assert(profile.binding_count == 14);

    binding = find(&profile, "MOVE_UP");
    assert(binding && binding->primary.device == PROFILE_DEVICE_KEYBOARD);
    assert(strcmp(binding->primary.code, "W") == 0);
    binding = find(&profile, "MOVE_LEFT");
    assert(binding && strcmp(binding->primary.code, "A") == 0);
    binding = find(&profile, "MOVE_DOWN");
    assert(binding && strcmp(binding->primary.code, "S") == 0);
    binding = find(&profile, "MOVE_RIGHT");
    assert(binding && strcmp(binding->primary.code, "D") == 0);

    binding = find(&profile, "CAMERA");
    assert(binding && binding->primary.device == PROFILE_DEVICE_MOUSE);
    assert(strcmp(binding->primary.code, "MOVE") == 0);

    binding = find(&profile, "L2");
    assert(binding && binding->primary.device == PROFILE_DEVICE_KEYBOARD);
    assert(strcmp(binding->primary.code, "CTRL") == 0);
    assert(binding->alternate_count == 0);

    binding = find(&profile, "R1");
    assert(binding && binding->primary.device == PROFILE_DEVICE_MOUSE);
    assert(strcmp(binding->primary.code, "LEFT") == 0);
    binding = find(&profile, "R2");
    assert(binding && strcmp(binding->primary.code, "RIGHT") == 0);
    binding = find(&profile, "R3");
    assert(binding && strcmp(binding->primary.code, "MIDDLE") == 0);

    assert(profile.sensitivity_x == 2.0f);
    assert(profile.sensitivity_y == 2.0f);
    assert(profile.smoothing == 0.0f);
    assert(!profile.invert_y);

    puts("profile defaults tests: PASS");
    return 0;
}
