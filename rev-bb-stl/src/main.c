/* main.c */
/* Sat 20 Jul 02:48:47 UTC 2024 */

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

void GPIOC_MODER_bang(void) {
    GPIOC->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
}

void GPIOD_MODER_bang(void) {
    /* PD15 is Blue LED - all 4x on PORT D pins 12-15 */
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
}

void set_USART6_BRR(void) {
    USART6->BRR = 0x138; /* 115200 */
}

void SETUP_USART6_CR1(void) {
    USART6->CR1 &= ~USART_CR1_UE;
    USART6->CR1 |= USART_CR1_RE;
    set_USART6_BRR();
    // USART6->BRR = 0x138; /* 115200 */
    USART6->CR1 |= USART_CR1_UE;
}

void map_AFR_USART6() {
    GPIOC->AFR[0] |=
        (0x8UL << GPIO_AFRL_AFSEL6_Pos) | (0x8UL << GPIO_AFRL_AFSEL7_Pos);
}

void enable_USART6() {
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
}

void enable_RCC_AHB1ENR_GPIOC() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}

void enable_RCC_AHB1ENR_GPIOD() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
}
void largest_USART6(void) {
    enable_RCC_AHB1ENR_GPIOC();
    enable_USART6();
    map_AFR_USART6();
    GPIOC_MODER_bang();
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7;
    SETUP_USART6_CR1();
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
 */

/* end. */
