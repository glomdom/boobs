#pragma once

#include "defines.h"

typedef struct clock {
    f64 start_time;
    f64 elapsed;
} clock;

BOOBS_API void clock_update(clock* clock);

BOOBS_API void clock_start(clock* clock);
BOOBS_API void clock_stop(clock* clock);
