#pragma once

#include "defines.h"

typedef union vec2_u {
    // x, y
    f32 elements[2];

    struct {
        union {
            f32 x, r, s, u;
        };

        union {
            f32 y, g, t, v;
        };
    };
} vec2;

typedef union vec3_u {
    union {
        // x, y, z
        f32 elements[3];

        struct {
            union {
                f32 x, r, s, u;
            };

            union {
                f32 y, g, t, v;
            };

            union {
                f32 z, b, p, w;
            };
        };
    };
} vec3;

typedef union vec4_u {
    // x, y, z, w
    f32 elements[4];

    union {
        struct {
            union {
                f32 x, r, s;
            };

            union {
                f32 y, g, t;
            };

            union {
                f32 z, b, p;
            };

            union {
                f32 w, a, q;
            };
        };
    };
} vec4;

typedef vec4 quat;

typedef union mat4_u {
    f32 data[16];
} mat4;
