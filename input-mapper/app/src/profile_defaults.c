#include "profile_model.h"

#include <string.h>

static ProfileInput keyboard(const char *code)
{
    ProfileInput input;
    memset(&input, 0, sizeof(input));
    input.device = PROFILE_DEVICE_KEYBOARD;
    size_t length = strlen(code);\n    if (length < sizeof(input.code)) memcpy(input.code, code, length + 1);
    return input;
}

static ProfileInput mouse(const char *code)
{
    ProfileInput input;
    memset(&input, 0, sizeof(input));
    input.device = PROFILE_DEVICE_MOUSE;
    strncpy(input.code, code, sizeof(input.code) - 1);
    return input;
}

bool input_mapper_init_default_profile(InputProfile *profile)
{
    if (!profile) return false;
    profile_init(profile, "pc-default", "PC ESTANDAR - TECLADO Y MOUSE");

    /* Movement and camera are modeled separately from controller buttons. */
    if (profile_add_binding(profile, "MOVE_UP", "MOVE UP", keyboard("W")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "MOVE_LEFT", "MOVE LEFT", keyboard("A")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "MOVE_DOWN", "MOVE DOWN", keyboard("S")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "MOVE_RIGHT", "MOVE RIGHT", keyboard("D")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "CAMERA", "CAMERA", mouse("MOVE")) != PROFILE_OK) return false;

    if (profile_add_binding(profile, "R1", "ATAQUE LIGERO", mouse("LEFT")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "R2", "ATAQUE PESADO", mouse("RIGHT")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "L1", "BLOQUEAR CON ESCUDO", keyboard("Q")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "L2", "APUNTAR / LANZAR HACHA", keyboard("CTRL")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "CROSS", "ESQUIVAR", keyboard("SPACE")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "INTERACT", "INTERACTUAR", keyboard("E")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "L3", "CORRER", keyboard("SHIFT")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "R3", "FIJAR OBJETIVO", mouse("MIDDLE")) != PROFILE_OK) return false;
    if (profile_add_binding(profile, "OPTIONS", "MENU", keyboard("ESC")) != PROFILE_OK) return false;

    profile->sensitivity_x = 2.0f;
    profile->sensitivity_y = 2.0f;
    profile->smoothing = 0.0f;
    profile->invert_y = false;
    return true;
}
