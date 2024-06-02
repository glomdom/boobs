#include <core/logger.h>
#include <core/asserts.h>
#include <platform/platform.h>

int main(void) {
    BOOBS_INFO("ping is in fact ping");
    BOOBS_WARN("idk a warn message");

    BOOBS_WARN("oh no i think we're gonna get an assertion failure");
    BOOBS_INFO("nvm");

    platform_state state;
    if (platform_startup(&state, "boobs testbed", 100, 100, 1280, 720)) {
        while (TRUE) {
            platform_pump_messages(&state);
        }
    }

    platform_shutdown(&state);

    return 0;
}