#include "boobs_math.h"

#include "platform/platform.h"

#include <math.h>
#include <stdlib.h>

static b8 rand_seeded = false;

f32 boobs_sin(f32 x) {
    return sinf(x);
}

f32 boobs_cos(f32 x) {
    return cosf(x);
}

f32 boobs_tan(f32 x) {
    return tanf(x);
}

f32 boobs_acos(f32 x) {
    return acosf(x);
}

f32 boobs_sqrt(f32 x) {
    return sqrtf(x);
}

f32 boobs_abs(f32 x) {
    return fabsf(x);
}

i32 boobs_random() {
    if (!rand_seeded) {
        srand((u32)platform_get_absolute_time());

        rand_seeded = true;
    }

    return rand();
}

i32 boobs_random_in_range(i32 min, i32 max) {
    if (!rand_seeded) {
        srand((u32)platform_get_absolute_time());
        rand_seeded = true;
    }
    return (rand() % (max - min + 1)) + min;
}

f32 boobs_frandom() {
    return (float)boobs_random() / (f32)RAND_MAX;
}

f32 boobs_frandom_in_range(f32 min, f32 max) {
    return min + ((float)boobs_random() / ((f32)RAND_MAX / (max - min)));
}
