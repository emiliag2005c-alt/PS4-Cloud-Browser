#include "profile_model.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static ProfileInput key(const char *code)
{
    ProfileInput input;
    input.device = PROFILE_DEVICE_KEYBOARD;
    snprintf(input.code, sizeof(input.code), "%s", code);
    return input;
}

static ProfileInput mouse(const char *code)
{
    ProfileInput input;
    input.device = PROFILE_DEVICE_MOUSE;
    snprintf(input.code, sizeof(input.code), "%s", code);
    return input;
}

int main(void)
{
    InputProfile p;
    profile_init(&p, "pc-default", "PC standard");
    assert(profile_add_binding(&p, "L2", "Aim / throw", key("CTRL")) == PROFILE_OK);
    assert(profile_add_binding(&p, "R1", "Light attack", mouse("LEFT")) == PROFILE_OK);
    assert(profile_add_binding(&p, "R2", "Heavy attack", mouse("RIGHT")) == PROFILE_OK);
    assert(profile_add_binding(&p, "L1", "Shield", key("Q")) == PROFILE_OK);
    assert(profile_add_binding(&p, "CROSS", "Dodge", key("SPACE")) == PROFILE_OK);
    assert(profile_add_binding(&p, "INTERACT", "Interact", key("E")) == PROFILE_OK);
    assert(profile_add_binding(&p, "L3", "Run", key("SHIFT")) == PROFILE_OK);
    assert(profile_add_binding(&p, "R3", "Lock target", mouse("MIDDLE")) == PROFILE_OK);

    assert(profile_find_conflict(&p, "L2", key("Q")) == PROFILE_CONFLICT_PRIMARY);
    assert(profile_find_conflict(&p, "L2", mouse("BUTTON4")) == PROFILE_CONFLICT_NONE);
    assert(profile_set_primary(&p, "L2", key("ALT")) == PROFILE_OK);
    assert(profile_add_alternate(&p, "L2", mouse("BUTTON4")) == PROFILE_OK);
    assert(profile_find_conflict(&p, "R1", mouse("BUTTON4")) == PROFILE_CONFLICT_ALTERNATE);
    assert(profile_remove_alternate(&p, "L2", 0) == PROFILE_OK);
    assert(profile_remove_binding(&p, "L3") == PROFILE_OK);
    assert(profile_remove_binding(&p, "L3") == PROFILE_ERR_NOT_FOUND);
    assert(profile_add_binding(&p, "R3", "Duplicate", mouse("MIDDLE")) == PROFILE_ERR_DUPLICATE);
    puts("profile model tests: PASS");
    return 0;
}
