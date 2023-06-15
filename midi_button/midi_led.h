#ifndef MIDI_LED_H
#define MIDI_LED_H

#include "stdint.h"

#define BRIGHTNESS_0 0
#define BRIGHTNESS_1 1
#define BRIGHTNESS_2 2
#define BRIGHTNESS_3 3
#define BRIGHTNESS_4 4

typedef void (*led_init_cb_t)(void);
typedef void (*led_apply_color_cb_t)(uint16_t r, uint16_t g, uint16_t b);

typedef struct _midi_led{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint32_t htmlcolor;
    uint8_t brightness;
    led_init_cb_t init;
    led_apply_color_cb_t apply_color;
}midi_led_t;

static inline void set_color(midi_led_t * led, uint32_t color) {
    if(led->brightness>4) led->brightness = 4;
    led->htmlcolor = color;
    led->r = ((color>>16)&0xFF)*led->brightness;
    led->g = ((color>>8)&0xFF)*led->brightness;
    led->b = (color&0xFF)*led->brightness;
    led->apply_color(led->r, led->g, led->b);
}

void midi_led_init(midi_led_t *led, led_init_cb_t init_hw);
void midi_let_set_color_cb(midi_led_t *led, led_apply_color_cb_t color_cb);
void midi_led_set_brightness(midi_led_t *led, uint8_t brightness);

#endif