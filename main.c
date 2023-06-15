#include "stm32f1xx.h"
#include "pinmacro.h"
#include "hardware.h"
#include "usb_lib.h"
#include "clock.h"
#include "midi_button_hw.h"
#include "midi_led.h"
#include "midi_button.h"
#include "stm32f1xx_it.h"
#include "midi_controller_config.h"

void __attribute__((weak)) _init(void){}
void __attribute__((weak)) SystemInit(void){}

void sleep(uint32_t time){
    while(time--)asm volatile("nop");
}
//Declare leds here
midi_led_t led0;
midi_led_t led1;

//Declare buttons here
midi_button_t button0;
midi_button_t button1;

//Declare msg buffers for buttons here
uint8_t prs_msg_buf0[MIDI_MSG_DEFAULT_LEN];
uint8_t rls_msg_buf0[MIDI_MSG_DEFAULT_LEN];
uint8_t lprs_msg_buf0[MIDI_MSG_DEFAULT_LEN];
uint8_t lrls_msg_buf0[MIDI_MSG_DEFAULT_LEN];

uint8_t prs_msg_buf1[MIDI_MSG_DEFAULT_LEN];
uint8_t rls_msg_buf1[MIDI_MSG_DEFAULT_LEN];
uint8_t lprs_msg_buf1[MIDI_MSG_DEFAULT_LEN];
uint8_t lrls_msg_buf1[MIDI_MSG_DEFAULT_LEN];

midi_button_t *buttons[BUTTONS_NUM];

int main(void){
    SysClockMax();
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN;
    //Init leds here
    midi_led_init(&led1, &init_button_0_led);
    midi_let_set_color_cb(&led1, &set_button_0_led_clrs);
    midi_led_init(&led0, &init_button_1_led);
    midi_let_set_color_cb(&led0, &set_button_1_led_clrs);
    //Init buttons here
    midi_button_init(&button0, &led0, &init_button_0);
    midi_button_init(&button1, &led1, &init_button_1);
    midi_button_set_poll_cb(&button0, &get_button_0_data);
    midi_button_set_poll_cb(&button1, &get_button_1_data);
    midi_button_init_buffers(&button0, prs_msg_buf0, rls_msg_buf0, lprs_msg_buf0, lrls_msg_buf0);
    midi_button_init_buffers(&button1, prs_msg_buf1, rls_msg_buf1, lprs_msg_buf1, lrls_msg_buf1);
    //Init button task handler
    buttons[0] = &button0;
    buttons[1] = &button1;
    
    SysTick->LOAD=72000-1;
    SysTick->VAL=72000-1;
    
    SysTick->CTRL=	SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
    USB_setup();
    usb_class_init();
    __enable_irq();
    

    while(1){
        for(uint8_t i=0; i<BUTTONS_NUM; i++) {

            if(buttons[i]->poll() == 0) apply_event(buttons[i], BUTTON_EVENT_DEBOUNCE);
            else apply_event(buttons[i], BUTTON_EVENT_RELEASE);

            //Handle debounce
            if(buttons[i]->button_timer == BUTTON_TIMER_DEBOUNCE_MS && buttons[i]->poll() == 0) {
                apply_event(buttons[i], BUTTON_EVENT_PRESS);
            }
            //Handle long press 
            if(buttons[i]->button_timer >= BUTTON_TIMER_LONG_PRESS_MS) apply_event(buttons[i], BUTTON_EVENT_LONG_PRESS);
        }
        usb_class_poll();
    }
}

void SysTick_Handler(void) {

    for(uint8_t i=0; i<BUTTONS_NUM; i++) {
        //Handle button timer increase
        if(buttons[i]->state == BUTTON_STATE_PRESSED || buttons[i]->state == BUTTON_STATE_DEBOUNCE) {
            buttons[i]->button_timer++;
        }
        else buttons[i]->button_timer = 0;
    }
}