#include "profile_model.h"

#include <string.h>

static bool copy_text(char *dst, size_t capacity, const char *src)
{
    size_t n;
    if (!dst || !src || capacity == 0) return false;
    n = strlen(src);
    if (n == 0 || n >= capacity) return false;
    memcpy(dst, src, n + 1);
    return true;
}

static ProfileBinding *find_binding(InputProfile *profile, const char *action)
{
    size_t i;
    if (!profile || !action) return NULL;
    for (i = 0; i < profile->binding_count; ++i)
        if (strcmp(profile->bindings[i].action, action) == 0)
            return &profile->bindings[i];
    return NULL;
}

static const ProfileBinding *find_binding_const(const InputProfile *profile,
                                                const char *action)
{
    size_t i;
    if (!profile || !action) return NULL;
    for (i = 0; i < profile->binding_count; ++i)
        if (strcmp(profile->bindings[i].action, action) == 0)
            return &profile->bindings[i];
    return NULL;
}

bool profile_input_equal(ProfileInput a, ProfileInput b)
{
    return a.device == b.device && strcmp(a.code, b.code) == 0;
}

void profile_init(InputProfile *profile, const char *id, const char *name)
{
    if (!profile) return;
    memset(profile, 0, sizeof(*profile));
    if (!copy_text(profile->id, sizeof(profile->id), id)) profile->id[0] = 0;
    if (!copy_text(profile->name, sizeof(profile->name), name)) profile->name[0] = 0;
    profile->sensitivity_x = 2.0f;
    profile->sensitivity_y = 2.0f;
    profile->smoothing = 0.0f;
}

ProfileConflict profile_find_conflict(const InputProfile *profile,
                                      const char *except_action,
                                      ProfileInput input)
{
    size_t i, j;
    if (!profile || !input.code[0]) return PROFILE_CONFLICT_NONE;
    for (i = 0; i < profile->binding_count; ++i) {
        const ProfileBinding *b = &profile->bindings[i];
        if (except_action && strcmp(b->action, except_action) == 0) continue;
        if (profile_input_equal(b->primary, input)) return PROFILE_CONFLICT_PRIMARY;
        for (j = 0; j < b->alternate_count; ++j)
            if (profile_input_equal(b->alternates[j], input))
                return PROFILE_CONFLICT_ALTERNATE;
    }
    return PROFILE_CONFLICT_NONE;
}

ProfileResult profile_add_binding(InputProfile *profile, const char *action,
                                  const char *label, ProfileInput input)
{
    ProfileBinding *b;
    if (!profile || !action || !label || !input.code[0] ||
        (input.device != PROFILE_DEVICE_KEYBOARD && input.device != PROFILE_DEVICE_MOUSE))
        return PROFILE_ERR_ARGUMENT;
    if (find_binding(profile, action)) return PROFILE_ERR_DUPLICATE;
    if (profile->binding_count >= PROFILE_MAX_BINDINGS) return PROFILE_ERR_FULL;
    b = &profile->bindings[profile->binding_count];
    memset(b, 0, sizeof(*b));
    if (!copy_text(b->action, sizeof(b->action), action) ||
        !copy_text(b->label, sizeof(b->label), label) ||
        !copy_text(b->primary.code, sizeof(b->primary.code), input.code))
        return PROFILE_ERR_INVALID;
    b->primary.device = input.device;
    ++profile->binding_count;
    return PROFILE_OK;
}

ProfileResult profile_set_primary(InputProfile *profile, const char *action,
                                  ProfileInput input)
{
    ProfileBinding *b = find_binding(profile, action);
    if (!b) return PROFILE_ERR_NOT_FOUND;
    if (!input.code[0] ||
        (input.device != PROFILE_DEVICE_KEYBOARD && input.device != PROFILE_DEVICE_MOUSE) ||
        strlen(input.code) >= PROFILE_CODE_LENGTH) return PROFILE_ERR_INVALID;
    b->primary = input;
    return PROFILE_OK;
}

ProfileResult profile_add_alternate(InputProfile *profile, const char *action,
                                    ProfileInput input)
{
    ProfileBinding *b = find_binding(profile, action);
    if (!b) return PROFILE_ERR_NOT_FOUND;
    if (!input.code[0] ||
        (input.device != PROFILE_DEVICE_KEYBOARD && input.device != PROFILE_DEVICE_MOUSE) ||
        strlen(input.code) >= PROFILE_CODE_LENGTH) return PROFILE_ERR_INVALID;
    if (b->alternate_count >= PROFILE_MAX_ALTERNATES) return PROFILE_ERR_FULL;
    b->alternates[b->alternate_count++] = input;
    return PROFILE_OK;
}

ProfileResult profile_remove_alternate(InputProfile *profile, const char *action,
                                       size_t alternate_index)
{
    ProfileBinding *b = find_binding(profile, action);
    size_t i;
    if (!b) return PROFILE_ERR_NOT_FOUND;
    if (alternate_index >= b->alternate_count) return PROFILE_ERR_ARGUMENT;
    for (i = alternate_index; i + 1 < b->alternate_count; ++i)
        b->alternates[i] = b->alternates[i + 1];
    memset(&b->alternates[--b->alternate_count], 0, sizeof(ProfileInput));
    return PROFILE_OK;
}

ProfileResult profile_remove_binding(InputProfile *profile, const char *action)
{
    size_t i;
    if (!profile || !action) return PROFILE_ERR_ARGUMENT;
    for (i = 0; i < profile->binding_count; ++i) {
        if (strcmp(profile->bindings[i].action, action) == 0) {
            size_t j;
            for (j = i; j + 1 < profile->binding_count; ++j)
                profile->bindings[j] = profile->bindings[j + 1];
            memset(&profile->bindings[--profile->binding_count], 0,
                   sizeof(ProfileBinding));
            return PROFILE_OK;
        }
    }
    return PROFILE_ERR_NOT_FOUND;
}
