#include "midi_button.h"
#include "midi_button_hw.h"
#include "midi_led.h"
#include "stddef.h"
#include "string.h"
#include "../usb_lib.h"
#include "midi_message_buffer.h"

extern midi_msg_buffer_t out_buf;

uint8_t id_counter = 0;

void midi_button_init(midi_button_t *button, midi_led_t *led, button_init_cb_t init_hw) {
    button->button_timer = 0;
    button->state = BUTTON_STATE_RELEASED;
    if(led != NULL) button->led = led;
    if(init_hw != NULL) {
        button->init = init_hw;
        button->init();
    }
    button->press_led_color = 0x9B00FF;
    button->long_press_led_color = 0xFF8700;
    button->release_led_color = 0x000000;
    button->id = id_counter;
    id_counter++;
}

void midi_button_set_poll_cb(midi_button_t *button, poll_button_cb_t poll_cb) {
    button->poll = poll_cb;
}

void apply_event(midi_button_t *button, button_event_t e) {
    switch (e) {
        case BUTTON_EVENT_DEBOUNCE:
            switch (button->state)
            {
                case BUTTON_STATE_RELEASED: {
                    button->state = BUTTON_STATE_DEBOUNCE;
                    //Debounce handling
                } break;
                default:
                    break;
            }
            break;
        case BUTTON_EVENT_PRESS:
            switch (button->state)
            {
                case BUTTON_STATE_DEBOUNCE: {
                    button->state = BUTTON_STATE_PRESSED;
                    //Short press handling
                    put_message(&out_buf, button->press_midi_msg, 4);
                    set_color(button->led, button->press_led_color);
                } break;
                default:
                    break;
            }
            break;
        case BUTTON_EVENT_LONG_PRESS:
            switch (button->state)
            {
                case BUTTON_STATE_PRESSED: {
                    button->state = BUTTON_STATE_LONG_PRESSED;
                    //Long press handling
                    put_message(&out_buf, button->long_press_midi_msg, 4);
                    set_color(button->led, button->long_press_led_color);
                } break;
                default:
                    break;
            }
            break;
        case BUTTON_EVENT_RELEASE:
            switch (button->state)
            {
                case BUTTON_STATE_PRESSED: {
                    button->state = BUTTON_STATE_RELEASED;
                    //Release after short press handling
                    put_message(&out_buf, button->release_midi_msg, 4);
                    set_color(button->led, button->release_led_color);
                } break;
                case BUTTON_STATE_LONG_PRESSED: {
                    button->state = BUTTON_STATE_RELEASED;
                    //Release after long press handling
                    put_message(&out_buf, button->long_press_end_midi_msg, 4);
                    set_color(button->led, button->release_led_color);
                } break;
                case BUTTON_STATE_DEBOUNCE: {
                    button->state = BUTTON_STATE_RELEASED;
                    //Release after long press handling
                    set_color(button->led, button->release_led_color);
                } break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void midi_button_set_press_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(button->press_midi_msg, msg, MIDI_MSG_DEFAULT_LEN);
}
void midi_button_set_release_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(button->release_midi_msg, msg, MIDI_MSG_DEFAULT_LEN);
}
void midi_button_set_lopress_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(button->long_press_midi_msg, msg, MIDI_MSG_DEFAULT_LEN);
}
void midi_button_set_lorelease_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(button->long_press_end_midi_msg, msg, MIDI_MSG_DEFAULT_LEN);
}

void midi_button_get_press_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(msg, button->press_midi_msg, MIDI_MSG_DEFAULT_LEN);
}
void midi_button_get_release_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(msg, button->release_midi_msg, MIDI_MSG_DEFAULT_LEN);
}
void midi_button_get_lopress_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(msg, button->long_press_midi_msg, MIDI_MSG_DEFAULT_LEN);
}
void midi_button_get_lorelease_midi_msg(midi_button_t *button, uint8_t *msg) {
    memcpy(msg, button->long_press_end_midi_msg, MIDI_MSG_DEFAULT_LEN);
}

void midi_button_init_buffers(midi_button_t *button, uint8_t * prs_msg, uint8_t * rls_msg, uint8_t *loprs_msg, uint8_t *lorls_msg) {
    button->press_midi_msg = prs_msg;
    button->release_midi_msg = rls_msg;
    button->long_press_midi_msg = loprs_msg;
    button->long_press_end_midi_msg = lorls_msg;
    //init with default values
    //Note on for press
    button->press_midi_msg[0] = 0x09;
    button->press_midi_msg[1] = 0x90;
    button->press_midi_msg[2] = button->id;
    button->press_midi_msg[3] = 127;
    //Note off for release
    button->release_midi_msg[0] = 0x08;
    button->release_midi_msg[1] = 0x80;
    button->release_midi_msg[2] = button->id;
    button->release_midi_msg[3] = 127;
    //disable long press
    button->long_press_midi_msg[0] = 0;
    button->long_press_midi_msg[1] = 0;
    button->long_press_midi_msg[2] = 0;
    button->long_press_midi_msg[3] = 0;
    //disable long releas
    button->long_press_end_midi_msg[0] = 0x08;
    button->long_press_end_midi_msg[1] = 0x80;
    button->long_press_end_midi_msg[2] = button->id;
    button->long_press_end_midi_msg[3] = 127;
}



