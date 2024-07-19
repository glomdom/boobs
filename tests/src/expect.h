#include <core/logger.h>
#include <math/boobs_math.h>

#define expect_should_be(expected, actual)                                                              \
    if (actual != expected) {                                                                           \
        BOOBS_ERROR("! expected %lld, but got: %lld in %s at line %d", expected, actual, __FILE__, __LINE__); \
        return false;                                                                                   \
    }

#define expect_should_not_be(expected, actual)                                                                   \
    if (actual == expected) {                                                                                    \
        BOOBS_ERROR("! expected %d != %d, but they are equal in %s at line %d", expected, actual, __FILE__, __LINE__); \
        return false;                                                                                            \
    }

#define expect_float_to_be(expected, actual)                                                        \
    if (boobs_abs(expected - actual) > 0.001f) {                                                         \
        BOOBS_ERROR("! expected %f, but got: %f. in %s at line %d", expected, actual, __FILE__, __LINE__); \
        return false;                                                                               \
    }

#define expect_to_be_true(actual)                                                      \
    if (actual != true) {                                                              \
        BOOBS_ERROR("! expected true, but got: false in %s at line %d", __FILE__, __LINE__); \
        return false;                                                                  \
    }

#define expect_to_be_false(actual)                                                          \
    if (actual != false) {                                                                  \
        BOOBS_ERROR("! expected false, but got: true in %s at line %d", __FILE__, __LINE__); \
        return false;                                                                       \
    }
