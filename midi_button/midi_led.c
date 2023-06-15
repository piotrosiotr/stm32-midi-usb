#include "midi_led.h"
#include "stddef.h"

void midi_led_init(midi_led_t *led, led_init_cb_t init_hw) {
    led->r = 0;
    led->g = 0;
    led->b = 0;
    led->htmlcolor = 0;
    led->brightness = BRIGHTNESS_1;
    led->init = init_hw;
    led->init();
}
void midi_let_set_color_cb(midi_led_t *led, led_apply_color_cb_t color_cb) {
    led->apply_color = color_cb;
}

void midi_led_set_brightness(midi_led_t *led, uint8_t brightness) {
    led->brightness = brightness;
}