#ifndef PS4_INPUT_PROFILE_MODEL_H
#define PS4_INPUT_PROFILE_MODEL_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PROFILE_MAX_BINDINGS 32
#define PROFILE_MAX_ALTERNATES 4
#define PROFILE_ID_LENGTH 32
#define PROFILE_NAME_LENGTH 64
#define PROFILE_CODE_LENGTH 24
#define PROFILE_LABEL_LENGTH 64

typedef enum ProfileDevice {
    PROFILE_DEVICE_KEYBOARD = 1,
    PROFILE_DEVICE_MOUSE = 2
} ProfileDevice;

typedef struct ProfileInput {
    ProfileDevice device;
    char code[PROFILE_CODE_LENGTH];
} ProfileInput;

typedef struct ProfileBinding {
    char action[PROFILE_ID_LENGTH];
    char label[PROFILE_LABEL_LENGTH];
    ProfileInput primary;
    ProfileInput alternates[PROFILE_MAX_ALTERNATES];
    size_t alternate_count;
} ProfileBinding;

typedef struct InputProfile {
    char id[PROFILE_ID_LENGTH];
    char name[PROFILE_NAME_LENGTH];
    ProfileBinding bindings[PROFILE_MAX_BINDINGS];
    size_t binding_count;
    float sensitivity_x;
    float sensitivity_y;
    float smoothing;
    bool invert_y;
} InputProfile;

typedef enum ProfileResult {
    PROFILE_OK = 0,
    PROFILE_ERR_ARGUMENT = -1,
    PROFILE_ERR_FULL = -2,
    PROFILE_ERR_NOT_FOUND = -3,
    PROFILE_ERR_DUPLICATE = -4,
    PROFILE_ERR_INVALID = -5
} ProfileResult;

typedef enum ProfileConflict {
    PROFILE_CONFLICT_NONE = 0,
    PROFILE_CONFLICT_PRIMARY = 1,
    PROFILE_CONFLICT_ALTERNATE = 2
} ProfileConflict;

void profile_init(InputProfile *profile, const char *id, const char *name);
ProfileResult profile_add_binding(InputProfile *profile, const char *action,
                                  const char *label, ProfileInput input);
ProfileResult profile_set_primary(InputProfile *profile, const char *action,
                                  ProfileInput input);
ProfileResult profile_add_alternate(InputProfile *profile, const char *action,
                                    ProfileInput input);
ProfileResult profile_remove_alternate(InputProfile *profile, const char *action,
                                       size_t alternate_index);
ProfileResult profile_remove_binding(InputProfile *profile, const char *action);
ProfileConflict profile_find_conflict(const InputProfile *profile,
                                      const char *except_action,
                                      ProfileInput input);
bool profile_input_equal(ProfileInput a, ProfileInput b);

#ifdef __cplusplus
}
#endif
#endif
