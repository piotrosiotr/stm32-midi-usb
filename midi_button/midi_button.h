#ifndef MIDI_BUTTON_H
#define MIDI_BUTTON_H

#include "stdint.h"
#include "midi_led.h"


#define BUTTON_TIMER_DEBOUNCE_MS 20 
#define BUTTON_TIMER_LONG_PRESS_MS 1000

typedef enum _button_state{
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_DEBOUNCE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_LONG_PRESSED,
}button_state_t;

typedef enum _button_event{
    BUTTON_EVENT_IDLE,
    BUTTON_EVENT_DEBOUNCE,
    BUTTON_EVENT_PRESS,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_RELEASE,
}button_event_t;

typedef void (*button_init_cb_t)(void);
typedef uint8_t (*poll_button_cb_t)(void);

typedef struct _midi_button{
    //General properties
    uint8_t id;
    //State machine properties
    button_state_t state;
    uint16_t button_timer;
    //Callbacks
    button_init_cb_t init;
    poll_button_cb_t poll;
    //MIDI messages for events
    uint8_t *press_midi_msg;
    uint8_t *release_midi_msg;
    uint8_t *long_press_midi_msg;
    uint8_t *long_press_end_midi_msg;
    //LED properties
    midi_led_t *led;
    uint32_t press_led_color;
    uint32_t release_led_color;
    uint32_t long_press_led_color;
}midi_button_t;

void apply_event(midi_button_t *button, button_event_t e);
void midi_button_init(midi_button_t *button, midi_led_t *led, button_init_cb_t init_hw);
void midi_button_set_poll_cb(midi_button_t *button, poll_button_cb_t poll_cb);

void midi_button_set_press_midi_msg(midi_button_t *button, uint8_t *msg);
void midi_button_set_release_midi_msg(midi_button_t *button, uint8_t *msg);
void midi_button_set_lopress_midi_msg(midi_button_t *button, uint8_t *msg);
void midi_button_set_lorelease_midi_msg(midi_button_t *button, uint8_t *msg);
void midi_button_init_buffers(midi_button_t *button, uint8_t * prs_msg, uint8_t * rls_msg, uint8_t *loprs_msg, uint8_t *lorls_msg);
#endif