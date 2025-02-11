#include "../../include/platform/lotus_input.h"
#include "../../include/platform/lotus_event.h"
#include "../../include/memory/lotus_memory.h"

static Lotus_Input_State internal_input_state = {0};

Lotus_Input_State* lotus_init_input(void) {
    internal_input_state.init = LOTUS_TRUE;
    return &internal_input_state;
}

void lotus_shutdown_input(void) {
    internal_input_state.init = LOTUS_FALSE;
}

void lotus_update_input(f64 delta_time) {
    if (!internal_input_state.init) return;

    // TODO: lotus_memory_api->copy() needed!
    memcpy(&internal_input_state.previous, &internal_input_state.current, sizeof(Llotus_Input_State));
    
    // reset current rawinput mouse deltas
    internal_input_state.current.mouse[0] = 0.0;
    internal_input_state.current.mouse[1] = 0.0;
}

ubyte lotus_key_is_up(Lotus_Keyboard_Key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.keys[key] == LOTUS_FALSE;
}

ubyte lotus_key_was_up(Lotus_Keyboard_Key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.keys[key] == LOTUS_FALSE;
}

ubyte lotus_key_is_down(Lotus_Keyboard_Key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.keys[key] == LOTUS_TRUE;
}

ubyte lotus_key_was_down(Lotus_Keyboard_Key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.keys[key] == LOTUS_TRUE;
}

ubyte lotus_button_is_up(Lotus_Mouse_Button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.buttons[button] == LOTUS_FALSE;
}

ubyte lotus_button_was_up(Lotus_Mouse_Button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.buttons[button] == LOTUS_FALSE;
}

ubyte lotus_button_is_down(Lotus_Mouse_Button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.buttons[button] == LOTUS_TRUE;
}

ubyte lotus_button_was_down(Lotus_Mouse_Button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.buttons[button] == LOTUS_TRUE;
}

void lotus_mouse_get_position(sbyte2* x, sbyte2* y) {
    if (!internal_input_state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = internal_input_state.current.mouse[0]; *y = internal_input_state.current.mouse[1];
}

void lotus_mouse_get_last_position(sbyte2* x, sbyte2* y) {
    if (!internal_input_state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = internal_input_state.previous.mouse[0]; *y = internal_input_state.previous.mouse[1];
}

void lotus_process_mouse_wheel_input(sbyte z_delta) {
    Lotus_Event event;
    event.sbyte[0] = z_delta;
    lotus_event_api->push_event(event, LOTUS_EVENT_MOUSE_WHEEL);
}

void lotus_process_mouse_move_input(sbyte2 x, sbyte2 y) {
    if (x != 0 || y != 0) {
        internal_input_state.previous.mouse[0] = internal_input_state.current.mouse[0];
        internal_input_state.previous.mouse[1] = internal_input_state.current.mouse[1];

        internal_input_state.current.mouse[0] += x; // WM_INPUT returns mouse deltas so just accumulate them
        internal_input_state.current.mouse[1] += y;

        Lotus_Event event;
        event.ubyte2[0] = internal_input_state.current.mouse[0];
        event.ubyte2[1] = internal_input_state.current.mouse[1];
        lotus_event_api->push_event(event, LOTUS_EVENT_MOUSE_MOVE);
    }
}

void lotus_process_key_input(Lotus_Keyboard_Key key, ubyte pressed) {
    if (internal_input_state.current.keys[key] != pressed) {
        internal_input_state.current.keys[key] = pressed;

        Lotus_Event event;
        event.ubyte2[0] = key;
        lotus_event_api->push_event(event, pressed ? LOTUS_EVENT_KEY_PRESSED : LOTUS_EVENT_KEY_RELEASED);
    }
}

void lotus_process_mouse_button_input(Lotus_Mouse_Button button, ubyte pressed) {
    if (internal_input_state.current.buttons[button] != pressed) {
        internal_input_state.current.buttons[button] = pressed;

        Lotus_Event event;
        event.ubyte2[0] = button;
        lotus_event_api->push_event(event, pressed ? LOTUS_EVENT_BUTTON_PRESSED : LOTUS_EVENT_BUTTON_RELEASED);
    }
}
