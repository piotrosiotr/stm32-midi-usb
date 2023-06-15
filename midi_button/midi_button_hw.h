#include "stdint.h"

void init_button_0(void);
void init_button_0_led(void);
void init_button_1(void);
void init_button_1_led(void);
void set_button_0_led_clrs(uint16_t r, uint16_t g, uint16_t b);
void set_button_1_led_clrs(uint16_t r, uint16_t g, uint16_t b);
uint8_t get_button_0_data(void);
uint8_t get_button_1_data(void);
void button_timer_init(void);