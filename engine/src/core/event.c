#include "core/event.h"

#include "core/logger.h"

#include "core/boobs_memory.h"
#include "containers/darray.h"

typedef struct registered_event {
    void* listener;
    PFN_on_event callback;
} registered_event;

typedef struct event_code_entry {
    registered_event* events;
} event_code_entry;

#define MAX_MESSAGE_CODES 16384

typedef struct event_system_state {
    event_code_entry registered[MAX_MESSAGE_CODES];
} event_system_state;

static event_system_state* state_ptr;

void event_system_initialize(u64* memory_requirement, void* state) {
    *memory_requirement = sizeof(event_system_state);
    if (state == 0) {
        return;
    }

    boobs_zero_memory(state, sizeof(state));

    state_ptr = state;
}

void event_system_shutdown(void* state) {
    if (state_ptr) {
        for (u16 i = 0; i < MAX_MESSAGE_CODES; ++i) {
            if (state_ptr->registered[i].events != 0) {
                darray_destroy(state_ptr->registered[i].events);

                state_ptr->registered[i].events = 0;
            }
        }
    }

    state_ptr = 0;

    BOOBS_INFO("event system has shutdown");
}

b8 event_register(u16 code, void* listener, PFN_on_event on_event) {
    if (!state_ptr) {
        return false;
    }

    if (state_ptr->registered[code].events == 0) {
        state_ptr->registered[code].events = darray_create(registered_event);
    }

    u64 registered_count = darray_length(state_ptr->registered[code].events);
    for (u64 i = 0; i < registered_count; ++i) {
        if (state_ptr->registered[code].events[i].listener == listener) {
            return false;
        }
    }

    registered_event event;
    event.listener = listener;
    event.callback = on_event;

    darray_push(state_ptr->registered[code].events, event);

    BOOBS_INFO("registered event with code 0x%x", code);

    return true;
}

b8 event_unregister(u16 code, void* listener, PFN_on_event on_event) {
    if (!state_ptr) {
        return false;
    }

    if (state_ptr->registered[code].events == 0) {
        return false;
    }

    u64 registered_count = darray_length(state_ptr->registered[code].events);
    for (u64 i = 0; i < registered_count; ++i) {
        registered_event e = state_ptr->registered[code].events[i];
        if (e.listener == listener && e.callback == on_event) {
            registered_event popped_event;

            darray_pop_at(state_ptr->registered[code].events, i, &popped_event);
            BOOBS_INFO("unregistered event with code 0x%x", code);

            return true;
        }
    }


    return false;
}

b8 event_fire(u16 code, void* sender, event_context context) {
    if (!state_ptr) {
        return false;
    }

    if (state_ptr->registered[code].events == 0) {
        return false;
    }

    u64 registered_count = darray_length(state_ptr->registered[code].events);
    for (u64 i = 0; i < registered_count; ++i) {
        registered_event e = state_ptr->registered[code].events[i];
        if (e.callback(code, sender, e.listener, context)) {
            return true;
        }
    }

    return false;
}
