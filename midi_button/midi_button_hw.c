#include "midi_button_hw.h"
#include <stm32f1xx.h>

#define MAX_BRIGHTNESS 1024


void init_button_0(void) {
    // PB13 - IPU
    GPIOB->CRH &= ~(GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
    GPIOB->CRH |= GPIO_CRH_CNF12_1; 
    GPIOB->ODR |= GPIO_ODR_ODR12; 
}

void init_button_0_led(void) {
    //Enable necessary clocks
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN;
    //Configure GPIOs AF PP
    // PA1
    GPIOA->CRL &= ~GPIO_CRL_CNF1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL |= GPIO_CRL_MODE1_1;
    // PA2
    GPIOA->CRL &= ~GPIO_CRL_CNF2;
    GPIOA->CRL |= GPIO_CRL_CNF2_1;
    GPIOA->CRL |= GPIO_CRL_MODE2_1;
    // PA3
    GPIOA->CRL &= ~GPIO_CRL_CNF3;
    GPIOA->CRL |= GPIO_CRL_CNF3_1;
    GPIOA->CRL |= GPIO_CRL_MODE3_1;
    //Configure PWM timer TIM2
    TIM2->PSC = 72;
    TIM2->ARR = MAX_BRIGHTNESS;
    TIM2->CCR4 = 0;
    TIM2->CCR3 = 0;
    TIM2->CCR2 = 0;
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM2->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2NE;
    TIM2->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3NE;
    TIM2->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4NP;
    TIM2->BDTR |= TIM_BDTR_MOE;
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE; 
    TIM2->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE; 
    TIM2->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE; 
    TIM2->CR1 &= ~TIM_CR1_DIR;
    TIM2->CR1 &= ~TIM_CR1_CMS;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void init_button_1(void) {
    // PB13 - IPU
    GPIOB->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOB->CRH |= GPIO_CRH_CNF13_1; 
    GPIOB->ODR |= GPIO_ODR_ODR13; 
}

void init_button_1_led(void) {
    //Enable necessary clocks
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN;
    //Configure GPIOs AF PP
    // PA7
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    GPIOA->CRL |= GPIO_CRL_MODE7_1;
    // PB0
    GPIOB->CRL &= ~GPIO_CRL_CNF0;
    GPIOB->CRL |= GPIO_CRL_CNF0_1;
    GPIOB->CRL |= GPIO_CRL_MODE0_1;
    // PB1
    GPIOB->CRL &= ~GPIO_CRL_CNF1;
    GPIOB->CRL |= GPIO_CRL_CNF1_1;
    GPIOB->CRL |= GPIO_CRL_MODE1_1;
    //Configure PWM timer TIM3
    TIM3->PSC = 72;
    TIM3->ARR = MAX_BRIGHTNESS;
    TIM3->CCR4 = 0;
    TIM3->CCR3 = 0;
    TIM3->CCR2 = 0;
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->EGR |= TIM_EGR_UG;
    TIM3->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2NE;
    TIM3->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3NE;
    TIM3->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4NP;
    TIM3->BDTR |= TIM_BDTR_MOE;
    
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE; 
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE; 
    TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE; 
    
    TIM3->CR1 &= ~TIM_CR1_DIR;
    TIM3->CR1 &= ~TIM_CR1_CMS;
    
    TIM3->CR1 |= TIM_CR1_CEN;
    
}

void set_button_0_led_clrs(uint16_t r, uint16_t g, uint16_t b) {
    // Clamp brightness values
    if(r>MAX_BRIGHTNESS) r = MAX_BRIGHTNESS;
    if(g>MAX_BRIGHTNESS) g = MAX_BRIGHTNESS;
    if(b>MAX_BRIGHTNESS) b = MAX_BRIGHTNESS;
    // Apply brightness values to CCRs
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM2->CCR3 = b;
    TIM2->CCR4 = r;
    TIM2->CCR2 = g;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void set_button_1_led_clrs(uint16_t r, uint16_t g, uint16_t b) {
    // Clamp brightness values
    if(r>MAX_BRIGHTNESS) r = MAX_BRIGHTNESS;
    if(g>MAX_BRIGHTNESS) g = MAX_BRIGHTNESS;
    if(b>MAX_BRIGHTNESS) b = MAX_BRIGHTNESS;
    // Apply brightness values to CCRs
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CCR3 = b;
    TIM3->CCR4 = r;
    TIM3->CCR2 = g;
    TIM3->CR1 |= TIM_CR1_CEN;
}



uint8_t get_button_0_data(void) {
    return (GPIOB->IDR & GPIO_IDR_IDR12)>>12;
}
uint8_t get_button_1_data(void) {
    return (GPIOB->IDR & GPIO_IDR_IDR13)>>13;
}
