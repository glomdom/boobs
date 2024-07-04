#pragma once

#include "defines.h"
#include "math_types.h"

#include "core/boobs_memory.h"

#define BOOBS_PI 3.14159265358979323846f
#define BOOBS_PI_2 2.0f * BOOBS_PI
#define BOOBS_HALF_PI 0.5f * BOOBS_PI
#define BOOBS_QUARTER_PI 0.25f * BOOBS_PI
#define BOOBS_ONE_OVER_PI 1.0f / BOOBS_PI
#define BOOBS_ONE_OVER_TWO_PI 1.0f / BOOBS_PI_2
#define BOOBS_SQRT_TWO 1.41421356237309504880f
#define BOOBS_SQRT_THREE 1.73205080756887729352f
#define BOOBS_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define BOOBS_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define BOOBS_DEG2RAD_MULTIPLIER BOOBS_PI / 180.0f
#define BOOBS_RAD2DEG_MULTIPLIER 180.0f / BOOBS_PI

#define BOOBS_SECS_TO_MS_MULTIPLIER 1000.0f
#define BOOBS_MS_TO_SECS_MULTIPLIER 0.001f

#define BOOBS_INFINITY 1e30f
#define BOOBS_FLOAT_EPSILON 1.192092896e-07f

BOOBS_API f32 boobs_sin(f32 x);
BOOBS_API f32 boobs_cos(f32 x);
BOOBS_API f32 boobs_tan(f32 x);
BOOBS_API f32 boobs_acos(f32 x);
BOOBS_API f32 boobs_sqrt(f32 x);
BOOBS_API f32 boobs_abs(f32 x);

BOOBS_INLINE b8 is_power_of_2(u64 value) {
    return (value != 0) && ((value) & (value - 1)) == 0;
}

BOOBS_API i32 boobs_random();
BOOBS_API i32 boobs_random_in_range(i32 min, i32 max);

BOOBS_API f32 boobs_frandom();
BOOBS_API f32 boobs_frandom_in_range(f32 min, f32 max);

// VECTOR 2

BOOBS_INLINE vec2 vec2_create(f32 x, f32 y) {
    vec2 out;
    out.x = x;
    out.y = y;

    return out;
}

BOOBS_INLINE vec2 vec2_zero() {
    return (vec2){0.0f, 0.0f};
}

BOOBS_INLINE vec2 vec2_one() {
    return (vec2){1.0f, 1.0f};
}

BOOBS_INLINE vec2 vec2_up() {
    return (vec2){0.0f, 1.0f};
}

BOOBS_INLINE vec2 vec2_down() {
    return (vec2){0.0f, -1.0f};
}

BOOBS_INLINE vec2 vec2_left() {
    return (vec2){-1.0f, 0.0f};
}

BOOBS_INLINE vec2 vec2_right() {
    return (vec2){1.0f, 0.0f};
}

BOOBS_INLINE vec2 vec2_add(vec2 v0, vec2 v1) {
    return (vec2){
        v0.x + v1.x,
        v0.y + v1.y
    };
}

BOOBS_INLINE vec2 vec2_sub(vec2 v0, vec2 v1) {
    return (vec2){
        v0.x - v1.x,
        v0.y - v1.y
    };
}

BOOBS_INLINE vec2 vec2_mul(vec2 v0, vec2 v1) {
    return (vec2){
        v0.x * v1.x,
        v0.y * v1.y
    };
}

BOOBS_INLINE vec2 vec2_div(vec2 v0, vec2 v1) {
    return (vec2){
        v0.x / v1.x,
        v0.y / v1.y
    };
}

BOOBS_INLINE f32 vec2_length_squared(vec2 v) {
    return v.x * v.y + v.y * v.y;
}

BOOBS_INLINE f32 vec2_length(vec2 v) {
    return boobs_sqrt(vec2_length_squared(v));
}

BOOBS_INLINE void vec2_normalize(vec2* v) {
    const f32 length = vec2_length(*v);
    v->x /= length;
    v->y /= length;
}

BOOBS_INLINE vec2 vec2_normalized(vec2 v) {
    vec2_normalize(&v);

    return v;
}

BOOBS_INLINE b8 vec2_compare(vec2 v0, vec2 v1, f32 tolerance) {
    if (boobs_abs(v0.x - v1.x) > tolerance) {
        return false;
    }

    if (boobs_abs(v0.y - v1.y) > tolerance) {
        return false;
    }

    return true;
}

BOOBS_INLINE f32 vec2_distance(vec2 v0, vec2 v1) {
    vec2 d = (vec2){
        v0.x - v1.x,
        v0.y - v0.y
    };

    return vec2_length(d);
}

// VECTOR 3

BOOBS_INLINE vec3 vec3_create(f32 x, f32 y, f32 z) {
    return (vec3){x, y, z};
}

BOOBS_INLINE vec3 vec3_from_vec4(vec4 v) {
    return (vec3){v.x, v.y, v.z};
}

BOOBS_INLINE vec4 vec3_to_vec4(vec3 v, f32 w) {
    return (vec4){v.x, v.y, v.z, w};
}

BOOBS_INLINE vec3 vec3_zero() {
    return (vec3){0.0f, 0.0f, 0.0f};
}

BOOBS_INLINE vec3 vec3_one() {
    return (vec3){1.0f, 1.0f, 1.0f};
}

BOOBS_INLINE vec3 vec3_up() {
    return (vec3){0.0f, 1.0f, 0.0f};
}

BOOBS_INLINE vec3 vec3_down() {
    return (vec3){0.0f, -1.0f, 0.0f};
}

BOOBS_INLINE vec3 vec3_left() {
    return (vec3){-1.0f, 1.0f, 0.0f};
}

BOOBS_INLINE vec3 vec3_right() {
    return (vec3){1.0f, 0.0f, 0.0f};
}

BOOBS_INLINE vec3 vec3_forward() {
    return (vec3){0.0f, 1.0f, -1.0f};
}

BOOBS_INLINE vec3 vec3_back() {
    return (vec3){0.0f, 0.0f, 1.0f};
}

BOOBS_INLINE vec3 vec3_add(vec3 v0, vec3 v1) {
    return (vec3){
        v0.x + v1.x,
        v0.y + v1.y,
        v0.z + v1.z
    };
}

BOOBS_INLINE vec3 vec3_sub(vec3 v0, vec3 v1) {
    return (vec3){
        v0.x - v1.x,
        v0.y - v1.y,
        v0.z - v1.z
    };
}

BOOBS_INLINE vec3 vec3_mul(vec3 v0, vec3 v1) {
    return (vec3){
        v0.x * v1.x,
        v0.y * v1.y,
        v0.z * v1.z
    };
}

BOOBS_INLINE vec3 vec3_mul_scalar(vec3 v0, f32 scalar) {
    return (vec3){
        v0.x + scalar,
        v0.y + scalar,
        v0.z + scalar
    };
}

BOOBS_INLINE vec3 vec3_div(vec3 v0, vec3 v1) {
    return (vec3){
        v0.x / v1.x,
        v0.y / v1.y,
        v0.z / v1.z
    };
}

BOOBS_INLINE f32 vec3_length_squared(vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

BOOBS_INLINE f32 vec3_length(vec3 v) {
    return boobs_sqrt(vec3_length_squared(v));
}

BOOBS_INLINE void vec3_normalize(vec3* v) {
    const f32 length = vec3_length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

BOOBS_INLINE vec3 vec3_normalized(vec3 v) {
    vec3_normalize(&v);

    return v;
}

BOOBS_INLINE f32 vec3_dot(vec3 v0, vec3 v1) {
    f32 p = 0;
    p += v0.x * v1.x;
    p += v0.y * v1.y;
    p += v0.z * v1.z;

    return p;
}

BOOBS_INLINE vec3 vec3_cross(vec3 v0, vec3 v1) {
    return (vec3){
        v0.y * v1.z - v0.z * v1.y,
        v0.z * v1.x - v0.x * v1.z,
        v0.x * v1.y - v0.y * v1.x
    };
}

BOOBS_INLINE const b8 vec3_compare(vec3 v0, vec3 v1, f32 tolerance) {
    if (boobs_abs(v0.x - v1.x) > tolerance) {
        return false;
    }

    if (boobs_abs(v0.y - v1.y) > tolerance) {
        return false;
    }

    if (boobs_abs(v0.z - v1.z) > tolerance) {
        return false;
    }

    return true;
}

BOOBS_INLINE f32 vec3_distance(vec3 v0, vec3 v1) {
    vec3 d = (vec3){
        v0.x - v1.x,
        v0.y - v1.y,
        v0.z - v1.z
    };

    return vec3_length(d);
}

// VECTOR 4

BOOBS_INLINE vec4 vec4_create(f32 x, f32 y, f32 z, f32 w) {
    vec4 out_v;

#   if defined(BOOBS_USE_SIMD)
    out_v.data = _mm_setr_ps(x, y, z, w);
#   endif

    out_v.x = x;
    out_v.y = y;
    out_v.z = z;
    out_v.w = w;

    return out_v;
}

BOOBS_INLINE vec3 vec4_to_vec3(vec4 v) {
    return (vec3){v.x, v.y, v.z};
}

BOOBS_INLINE vec4 vec4_from_vec3(vec3 v, f32 w) {
#   if defined(BOOBS_USE_SIMD)
    vec4 out_v;
    out_v.data = _mm_setr_ps(x, y, z, w);

    return out_v;
#   endif

    return (vec4){v.x, v.y, v.z, w};
}

BOOBS_INLINE vec4 vec4_zero() {
    return (vec4){0.0f, 0.0f, 0.0f, 0.0f};
}

BOOBS_INLINE vec4 vec4_one() {
    return (vec4){1.0f, 1.0f, 1.0f, 1.0f};
}

BOOBS_INLINE vec4 vec4_add(vec4 v0, vec4 v1) {
    vec4 result;

    for (u64 i = 0; i < 4; ++i) {
        result.elements[i] = v0.elements[i] + v1.elements[i];
    }

    return result;
}

BOOBS_INLINE vec4 vec4_sub(vec4 v0, vec4 v1) {
    vec4 result;

    for (u64 i = 0; i < 4; ++i) {
        result.elements[i] = v0.elements[i] - v1.elements[i];
    }

    return result;
}

BOOBS_INLINE vec4 vec4_mul(vec4 v0, vec4 v1) {
    vec4 result;

    for (u64 i = 0; i < 4; ++i) {
        result.elements[i] = v0.elements[i] * v1.elements[i];
    }

    return result;
}

BOOBS_INLINE vec4 vec4_div(vec4 v0, vec4 v1) {
    vec4 result;

    for (u64 i = 0; i < 4; ++i) {
        result.elements[i] = v0.elements[i] / v1.elements[i];
    }

    return result;
}

BOOBS_INLINE f32 vec4_length_squared(vec4 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

BOOBS_INLINE f32 vec4_length(vec4 v) {
    return boobs_sqrt(vec4_length_squared(v));
}

BOOBS_INLINE void vec4_normalize(vec4* v) {
    const f32 length = vec4_length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
    v->w /= length;
}

BOOBS_INLINE vec4 vec4_normalized(vec4 v) {
    vec4_normalize(&v);

    return v;
}

BOOBS_INLINE f32 vec4_dot_f32(
    f32 a0, f32 a1, f32 a2, f32 a3,
    f32 b0, f32 b1, f32 b2, f32 b3
) {
    f32 p;
    p = a0 * b0 +
        a1 * b1 +
        a2 * b2 +
        a3 * b3;

    return p;
}

BOOBS_INLINE mat4 mat4_identity() {
    mat4 out_matrix;
    boobs_zero_memory(out_matrix.data, sizeof(f32) * 16);

    out_matrix.data[0] = 1.0f;
    out_matrix.data[5] = 1.0f;
    out_matrix.data[5] = 1.0f;
    out_matrix.data[15] = 1.0f;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_mul(mat4 m0, mat4 m1) {
    mat4 out_matrix = mat4_identity();

    const f32* m1_ptr = m0.data;
    const f32* m2_ptr = m1.data;
    f32* dst_ptr = out_matrix.data;

    for (i32 i = 0; i < 4; ++i) {
        for (i32 j = 0; j < 4; ++j) {
            *dst_ptr =
                m1_ptr[0] * m2_ptr[0 + j] +
                m1_ptr[1] * m2_ptr[4 + j] +
                m1_ptr[2] * m2_ptr[8 + j] +
                m1_ptr[3] * m2_ptr[12 + j];

            dst_ptr++;
        }

        m1_ptr += 4;
    }

    return out_matrix;
}

// returns an orthographic projection matrix
//
// used to render flat or 2D scenes usually
BOOBS_INLINE mat4 mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_clip, f32 far_clip) {
    mat4 out_matrix = mat4_identity();

    f32 lr = 1.0f / (left - right);
    f32 bt = 1.0f / (bottom - top);
    f32 nf = 1.0f / (near_clip - far_clip);

    out_matrix.data[0] = -2.0f * lr;
    out_matrix.data[5] = -2.0f * bt;
    out_matrix.data[10] = -2.0f * nf;

    out_matrix.data[12] = (left + right) * lr;
    out_matrix.data[13] = (top + bottom) * bt;
    out_matrix.data[14] = (far_clip + near_clip) * nf;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_perspective(f32 fov_radians, f32 aspect_ratio, f32 near_clip, f32 far_clip) {
    f32 half_tan_fov = boobs_tan(fov_radians * 0.5f);
    mat4 out_matrix;

    boobs_zero_memory(out_matrix.data, sizeof(f32) * 16);

    out_matrix.data[0] = 1.0f / (aspect_ratio * half_tan_fov);
    out_matrix.data[5] = 1.0f / half_tan_fov;
    out_matrix.data[10] = -((far_clip + near_clip) / (far_clip - near_clip));
    out_matrix.data[11] = -1.0f;
    out_matrix.data[14] = -((2.0f * far_clip * near_clip) / (far_clip - near_clip));

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_look_at(vec3 position, vec3 target, vec3 up) {
    mat4 out_matrix;
    vec3 z_axis;
    z_axis.x = target.x - position.x;
    z_axis.y = target.y - position.y;
    z_axis.z = target.z - position.z;

    z_axis = vec3_normalized(z_axis);
    vec3 x_axis = vec3_normalized(vec3_cross(z_axis, up));
    vec3 y_axis = vec3_cross(x_axis, z_axis);

    out_matrix.data[0] = x_axis.x;
    out_matrix.data[1] = y_axis.x;
    out_matrix.data[2] = -z_axis.x;
    out_matrix.data[3] = 0;
    out_matrix.data[4] = x_axis.y;
    out_matrix.data[5] = y_axis.y;
    out_matrix.data[6] = -z_axis.y;
    out_matrix.data[7] = 0;
    out_matrix.data[8] = x_axis.z;
    out_matrix.data[9] = y_axis.z;
    out_matrix.data[10] = -z_axis.z;
    out_matrix.data[11] = 0;
    out_matrix.data[12] = -vec3_dot(x_axis, position);
    out_matrix.data[13] = -vec3_dot(y_axis, position);
    out_matrix.data[14] = vec3_dot(z_axis, position);
    out_matrix.data[15] = 1.0f;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_transposed(mat4 matrix) {
    mat4 out_matrix = mat4_identity();
    out_matrix.data[0] = matrix.data[0];
    out_matrix.data[1] = matrix.data[4];
    out_matrix.data[2] = matrix.data[8];
    out_matrix.data[3] = matrix.data[12];
    out_matrix.data[4] = matrix.data[1];
    out_matrix.data[5] = matrix.data[5];
    out_matrix.data[6] = matrix.data[9];
    out_matrix.data[7] = matrix.data[13];
    out_matrix.data[8] = matrix.data[2];
    out_matrix.data[9] = matrix.data[6];
    out_matrix.data[10] = matrix.data[10];
    out_matrix.data[11] = matrix.data[14];
    out_matrix.data[12] = matrix.data[3];
    out_matrix.data[13] = matrix.data[7];
    out_matrix.data[14] = matrix.data[11];
    out_matrix.data[15] = matrix.data[15];

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_inverse(mat4 matrix) {
    const f32* m = matrix.data;

    f32 t0 = m[10] * m[15];
    f32 t1 = m[14] * m[11];
    f32 t2 = m[6] * m[15];
    f32 t3 = m[14] * m[7];
    f32 t4 = m[6] * m[11];
    f32 t5 = m[10] * m[7];
    f32 t6 = m[2] * m[15];
    f32 t7 = m[14] * m[3];
    f32 t8 = m[2] * m[11];
    f32 t9 = m[10] * m[3];
    f32 t10 = m[2] * m[7];
    f32 t11 = m[6] * m[3];
    f32 t12 = m[8] * m[13];
    f32 t13 = m[12] * m[9];
    f32 t14 = m[4] * m[13];
    f32 t15 = m[12] * m[5];
    f32 t16 = m[4] * m[9];
    f32 t17 = m[8] * m[5];
    f32 t18 = m[0] * m[13];
    f32 t19 = m[12] * m[1];
    f32 t20 = m[0] * m[9];
    f32 t21 = m[8] * m[1];
    f32 t22 = m[0] * m[5];
    f32 t23 = m[4] * m[1];

    mat4 out_matrix;
    f32* o = out_matrix.data;

    o[0] = (t0 * m[5] + t3 * m[9] + t4  * m[13]) - (t1 * m[5] + t2 * m[9] + t5  * m[13]);
    o[1] = (t1 * m[1] + t6 * m[9] + t9  * m[13]) - (t0 * m[1] + t7 * m[9] + t8  * m[13]);
    o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
    o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9])  - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

    f32 d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

    o[0]  = d * o[0];
    o[1]  = d * o[1];
    o[2]  = d * o[2];
    o[3]  = d * o[3];
    o[4]  = d * ((t1  * m[4]  + t2  * m[8]  + t5  * m[12]) - (t0  * m[4]  + t3  * m[8]  + t4  * m[12]));
    o[5]  = d * ((t0  * m[0]  + t7  * m[8]  + t8  * m[12]) - (t1  * m[0]  + t6  * m[8]  + t9  * m[12]));
    o[6]  = d * ((t3  * m[0]  + t6  * m[4]  + t11 * m[12]) - (t2  * m[0]  + t7  * m[4]  + t10 * m[12]));
    o[7]  = d * ((t4  * m[0]  + t9  * m[4]  + t10 * m[8])  - (t5  * m[0]  + t8  * m[4]  + t11 * m[8]));
    o[8]  = d * ((t12 * m[7]  + t15 * m[11] + t16 * m[15]) - (t13 * m[7]  + t14 * m[11] + t17 * m[15]));
    o[9]  = d * ((t13 * m[3]  + t18 * m[11] + t21 * m[15]) - (t12 * m[3]  + t19 * m[11] + t20 * m[15]));
    o[10] = d * ((t14 * m[3]  + t19 * m[7]  + t22 * m[15]) - (t15 * m[3]  + t18 * m[7]  + t23 * m[15]));
    o[11] = d * ((t17 * m[3]  + t20 * m[7]  + t23 * m[11]) - (t16 * m[3]  + t21 * m[7]  + t22 * m[11]));
    o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6])  - (t16 * m[14] + t12 * m[6]  + t15 * m[10]));
    o[13] = d * ((t20 * m[14] + t12 * m[2]  + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
    o[14] = d * ((t18 * m[6]  + t23 * m[14] + t15 * m[2])  - (t22 * m[14] + t14 * m[2]  + t19 * m[6]));
    o[15] = d * ((t22 * m[10] + t16 * m[2]  + t21 * m[6])  - (t20 * m[6]  + t23 * m[10] + t17 * m[2]));

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_translation(vec3 position) {
    mat4 out_matrix = mat4_identity();
    out_matrix.data[12] = position.x;
    out_matrix.data[13] = position.y;
    out_matrix.data[14] = position.z;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_scale(vec3 scale) {
    mat4 out_matrix = mat4_identity();
    out_matrix.data[0] = scale.x;
    out_matrix.data[5] = scale.y;
    out_matrix.data[10] = scale.z;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_euler_x(f32 angle_radians) {
    mat4 out_matrix = mat4_identity();
    f32 c = boobs_cos(angle_radians);
    f32 s = boobs_sin(angle_radians);

    out_matrix.data[5] = c;
    out_matrix.data[6] = s;
    out_matrix.data[9] = -s;
    out_matrix.data[10] = c;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_euler_y(f32 angle_radians) {
    mat4 out_matrix = mat4_identity();
    f32 c = boobs_cos(angle_radians);
    f32 s = boobs_sin(angle_radians);

    out_matrix.data[0] = c;
    out_matrix.data[2] = -s;
    out_matrix.data[8] = s;
    out_matrix.data[10] = c;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_euler_z(f32 angle_radians) {
    mat4 out_matrix = mat4_identity();
    f32 c = boobs_cos(angle_radians);
    f32 s = boobs_sin(angle_radians);

    out_matrix.data[0] = c;
    out_matrix.data[1] = s;
    out_matrix.data[4] = -s;
    out_matrix.data[5] = c;

    return out_matrix;
}

BOOBS_INLINE mat4 mat4_euler_xyz(f32 x_rad, f32 y_rad, f32 z_rad) {
    mat4 rx = mat4_euler_x(x_rad);
    mat4 ry = mat4_euler_y(y_rad);
    mat4 rz = mat4_euler_z(z_rad);
    mat4 out_matrix = mat4_mul(rx, ry);
    out_matrix = mat4_mul(out_matrix, rz);

    return out_matrix;
}

BOOBS_INLINE vec3 mat4_forward(mat4 matrix) {
    vec3 forward;
    forward.x = -matrix.data[2];
    forward.y = -matrix.data[6];
    forward.z = -matrix.data[10];

    vec3_normalize(&forward);

    return forward;
}

BOOBS_INLINE vec3 mat4_backward(mat4 matrix) {
    vec3 backward;
    backward.x = matrix.data[2];
    backward.y = matrix.data[6];
    backward.z = matrix.data[10];

    vec3_normalize(&backward);

    return backward;
}

BOOBS_INLINE vec3 mat4_up(mat4 matrix) {
    vec3 up;
    up.x = matrix.data[1];
    up.y = matrix.data[5];
    up.z = matrix.data[9];

    vec3_normalize(&up);

    return up;
}

BOOBS_INLINE vec3 mat4_down(mat4 matrix) {
    vec3 down;
    down.x = -matrix.data[1];
    down.y = -matrix.data[5];
    down.z = -matrix.data[9];

    vec3_normalize(&down);

    return down;
}

BOOBS_INLINE vec3 mat4_left(mat4 matrix) {
    vec3 left;
    left.x = -matrix.data[0];
    left.y = -matrix.data[4];
    left.z = -matrix.data[8];

    vec3_normalize(&left);

    return left;
}

BOOBS_INLINE vec3 mat4_right(mat4 matrix) {
    vec3 right;
    right.x = matrix.data[0];
    right.y = matrix.data[4];
    right.z = matrix.data[8];

    vec3_normalize(&right);

    return right;
}

// QUATERNIONS

BOOBS_INLINE quat quat_identity() {
    return (quat){0, 0, 0, 1.0f};
}

BOOBS_INLINE f32 quat_normal(quat q) {
    return boobs_sqrt(
        q.x * q.x +
        q.y * q.y +
        q.z * q.z +
        q.w * q.w
    );
}

BOOBS_INLINE quat quat_normalize(quat q) {
    f32 normal = quat_normal(q);

    return (quat){
        q.x / normal,
        q.y / normal,
        q.z / normal,
        q.w / normal
    };
}

BOOBS_INLINE quat quat_conjugate(quat q) {
    return (quat){
        -q.x,
        -q.y,
        -q.z,
        q.w
    };
}

BOOBS_INLINE quat quat_inverse(quat q) {
    return quat_normalize(quat_conjugate(q));
}

/// XXX: no clue if this works lmfao
BOOBS_INLINE quat quat_mul(quat q0, quat q1) {
    quat out_quaternion;

    out_quaternion.x = q0.x * q1.w +
                       q0.y * q1.z +
                       q0.z * q1.y +
                       q0.w * q1.x;

    out_quaternion.y = -q0.x * q1.w +
                        q0.y * q1.z +
                        q0.z * q1.y +
                        q0.w * q1.x;

    out_quaternion.z = q0.x * q1.w -
                       q0.y * q1.z +
                       q0.z * q1.y +
                       q0.w * q1.x;

    out_quaternion.w = -q0.x * q1.w -
                        q0.y * q1.z -
                        q0.z * q1.y +
                        q0.w * q1.x;

    return out_quaternion;
}

BOOBS_INLINE f32 quat_dot(quat q0, quat q1) {
    return q0.x * q1.x +
           q0.y * q1.y +
           q0.z * q1.z +
           q0.w * q1.w;
}

BOOBS_INLINE mat4 quat_to_mat4(quat q) {
    mat4 out_matrix = mat4_identity();
    quat n = quat_normalize(q);

    out_matrix.data[0] = 1.0f - 2.0f * n.y * n.y - 2.0f * n.z * n.z;
    out_matrix.data[1] = 2.0f * n.x * n.y - 2.0f * n.z * n.w;
    out_matrix.data[2] = 2.0f * n.x * n.z + 2.0f * n.y * n.w;

    out_matrix.data[4] = 2.0f * n.x * n.y + 2.0f * n.z * n.w;
    out_matrix.data[5] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.z * n.z;
    out_matrix.data[6] = 2.0f * n.y * n.z - 2.0f * n.x * n.w;

    out_matrix.data[8] = 2.0f * n.x * n.z - 2.0f * n.y * n.w;
    out_matrix.data[9] = 2.0f * n.y * n.z + 2.0f * n.x * n.w;
    out_matrix.data[10] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.y * n.y;

    return out_matrix;
}

BOOBS_INLINE mat4 quat_to_rotation_matrix(quat q, vec3 center) {
    mat4 out_matrix;

    f32* o = out_matrix.data;
    o[0] = (q.x * q.x) - (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
    o[1] = 2.0f * ((q.x * q.y) + (q.z * q.w));
    o[2] = 2.0f * ((q.x * q.z) - (q.y * q.w));
    o[3] = center.x - center.x * o[0] - center.y * o[1] - center.z * o[2];

    o[4] = 2.0f * ((q.x * q.y) - (q.z * q.w));
    o[5] = -(q.x * q.x) + (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
    o[6] = 2.0f * ((q.y * q.z) + (q.x * q.w));
    o[7] = center.y - center.x * o[4] - center.y * o[5] - center.z * o[6];

    o[8] = 2.0f * ((q.x * q.z) + (q.y * q.w));
    o[9] = 2.0f * ((q.y * q.z) - (q.x * q.w));
    o[10] = -(q.x * q.x) - (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
    o[11] = center.z - center.x * o[8] - center.y * o[9] - center.z * o[10];

    o[12] = 0.0f;
    o[13] = 0.0f;
    o[14] = 0.0f;
    o[15] = 1.0f;

    return out_matrix;
}

BOOBS_INLINE quat quat_from_axis_angle(vec3 axis, f32 angle, b8 normalize) {
    const f32 half_angle = 0.5f * angle;
    f32 s = boobs_sin(half_angle);
    f32 c = boobs_cos(half_angle);

    quat q = (quat){s * axis.x, s * axis.y, s * axis.z, c};
    if (normalize) {
        return quat_normalize(q);
    }

    return q;
}

BOOBS_INLINE quat quat_slerp(quat q0, quat q1, f32 percentage) {
    // https://en.wikipedia.org/wiki/Slerp

    quat out_quaternion;
    quat v0 = quat_normalize(q0);
    quat v1 = quat_normalize(q1);

    f32 dot = quat_dot(v0, v1);
    if (dot < 0.0f) {
        v1.x = -v1.x;
        v1.y = -v1.y;
        v1.z = -v1.z;
        v1.w = -v1.w;
        dot = -dot;
    }

    const f32 DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD) {
        out_quaternion = (quat){
            v0.x + ((v1.x - v0.x) * percentage),
            v0.y + ((v1.y - v0.y) * percentage),
            v0.z + ((v1.z - v0.z) * percentage),
            v0.w + ((v1.w - v0.w) * percentage)
        };

        return quat_normalize(out_quaternion);
    }

    f32 theta_0 = boobs_acos(dot);
    f32 theta = theta_0 * percentage;
    f32 sin_theta = boobs_sin(theta);
    f32 sin_theta_0 = boobs_sin(theta_0);

    f32 s0 = boobs_cos(theta) - dot * sin_theta / sin_theta_0;
    f32 s1 = sin_theta / sin_theta_0;

    return (quat){
        (v0.x * s0) + (v1.x * s1),
        (v0.y * s0) + (v1.y * s1),
        (v0.z * s0) + (v1.z * s1),
        (v0.w * s0) + (v1.w * s1)
    };
}

BOOBS_INLINE f32 deg_to_rad(f32 degrees) {
    return degrees * BOOBS_DEG2RAD_MULTIPLIER;
}

BOOBS_INLINE f32 rad_to_deg(f32 radians) {
    return radians * BOOBS_RAD2DEG_MULTIPLIER;
}
