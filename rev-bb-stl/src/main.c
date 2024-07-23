/* main.c */

/***
 * large revision -
 * Tue 23 Jul 16:47:38 UTC 2024
 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

/****
 * PD15  LD6  Blue
 * PD12  LD4  Green  #  PD13 LD3  Orange  # PD14 LD5  Red
 ***/

#include <stdbool.h>
#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio_ex.h>

#include <delays.h>

#define pinNumber 15 // PD15
#define SLOW_CONST 3

// ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports
// RCC_APB2ENR  112, 146    GPIOx_CRH  172

void blink() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void GPIOD_MODER_bang(void) {
    /* PD15 is Blue LED - all 4x on PORT D pins 12-15 */
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
}

void initUSART6(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    GPIOC->MODER |= GPIO_MODER_MODER6_1 |
        GPIO_MODER_MODER7_1;
    GPIOC->AFR[0] |=
        (0x8UL << GPIO_AFRL_AFSEL6_Pos) |
            (0x8UL << GPIO_AFRL_AFSEL7_Pos);
    USART6->CR1 &= ~USART_CR1_UE;
    USART6->CR1 |= USART_CR1_RE;
    USART6->BRR = 0x138; /* 115200 */
}

void enable_RCC_AHB1ENR_GPIOD() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
}

void primary(void) {
    enable_RCC_AHB1ENR_GPIOD();
    GPIOD_MODER_bang();
    GPIOD->OTYPER = 0;
    GPIOD->OSPEEDR = 0;
    sayHi();
}

int main(void) {
    primary();
    initUSART6();
    ldelayed();
    monitor();
    return 0;
}

/* end. */
