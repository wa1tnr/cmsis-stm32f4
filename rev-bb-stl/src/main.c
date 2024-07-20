
/****
 *
 * Wed 17 Jul 05:51:07 UTC 2024
 *
 * STATUS - although the code was guesswork, at a minimum,
 * it is operational (the blue LED continues to blink after
 * the 'fiasco' section of the code WRT USART6).
 *
 *    monitor() is now called after the USART6 routines (the
 *    function, largest_USART6() .. which calls all else wrt
 *    USART6 setups).
 *
 */

/***
 *
 * PD15  LD6  Blue
 *
 * PD12  LD4  Green  #  PD13 LD3  Orange  # PD14 LD5  Red
 *
 ***/

/***
 *
 * NEWS - blue LED stm32f407 Discovery now blinks blue LED.
 *
 ***/

#include <stdbool.h>
#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio_ex.h>

#define pinNumber 15 // PD15
#define SLOW_CONST 3

// ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports
// RCC_APB2ENR  112, 146    GPIOx_CRH  172

extern void blink_once();
extern void slowest();
extern void blinkDelayOffToOn();

void blink() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void GPIOC_MODER_bang(void) {
    GPIOC->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7;
}

void SETUP_USART6_CR1(void) {
    USART6->CR1 &= ~USART_CR1_UE;
    USART6->CR1 |= USART_CR1_RE;
    USART6->BRR = 0x138; /* 115200 */
    USART6->CR1 |= USART_CR1_UE;
}

void SET_GPIOC_MODER_PC6_PC7_ALT_A() {
    GPIOC->AFR[0] |=
        (0x8UL << GPIO_AFRL_AFSEL6_Pos) | (0x8UL << GPIO_AFRL_AFSEL7_Pos);
}

void enable_USART6() {
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
}

void enable_RCC_AHB1ENR_GPIOC() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}

/* TODO: give this a better name than largest foo */

void largest_USART6(void) {
    enable_USART6();
    SET_GPIOC_MODER_PC6_PC7_ALT_A();
    GPIOC_MODER_bang();
    SETUP_USART6_CR1();
}

void GPIOD__MODER____GPIO_MODER_MODER15_0() {
    /* PD15 is Blue LED - all 4x on PORT D pins 12-15 */
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
}

// c-lang-format off
void enable_RCC_AHB1ENR_GPIOD() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
}
// c-lang-format on

extern void sayHi();

void primary(void) {
    enable_RCC_AHB1ENR_GPIOD();
    GPIOD__MODER____GPIO_MODER_MODER15_0();
    GPIOD->OTYPER = 0;
    GPIOD->OSPEEDR = 0;
    enable_RCC_AHB1ENR_GPIOC();
    sayHi();
}

extern void ldelayed();
extern void monitor();
int main(void) {
    primary();
    largest_USART6();
    /*******************************************************************/
    /* KEEP:  do NOT remove COMMENT saying largest_USART6() goes here: */
    /* KEEP:  largest_USART6();                                        */
    /*******************************************************************/
    ldelayed();
    monitor();
    return 0;
}

/****
 *
 *
 *
 *
 * *********************************************************
 * *********************************************************
 * *********************************************************
 * *********************************************************
 * *********************************************************
 *   Menu: Go > Switch Editor   GO SWITCH EDITOR and GO SWITCH GROUP!
 *   Menu: Go > Switch Editor   GO SWITCH EDITOR and GO SWITCH GROUP!
 *
 * *********************************************************
 * *********************************************************
 * *********************************************************
 * *********************************************************
 * *********************************************************
 * *********************************************************
 * *********************************************************
 *
 *
 *
 *
 */

/* end. */
