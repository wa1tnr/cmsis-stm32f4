/* gpio.c */
/* Tue  6 Aug 00:23:18 UTC 2024 */

/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

#include <delays.h>
#include <morse.h>
#include <stm32f4xx.h>

/* ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports */
/* RCC_APB2ENR  112, 146    GPIOx_CRH  172 */

void resetBlueLED() {
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void setBlueLED() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
}

void sendDit() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    ditDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
    ditDelayOffToOn();
}

void turn_on_LED_forever() {
    GPIOD->BSRR = GPIO_BSRR_BR_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BS_15;
}

void turn_OUT_LED_forever() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void initGPIO(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
    GPIOD->OTYPER = 0;
    GPIOD->OSPEEDR = 0;
}

void blink() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void quickBlinks() {
    for (int bln = 3; bln > 0; bln--) {
        blink_once();
    }
}

void sendMorseMsgNO() {
    sendMorseLtrN();
    sendMorseSpace();
    sendMorseLtrO();
    sendMorseSpace();
    sendMorseWSpace();
}

void threeQBlinks(void) {
    for (int qblinks = 3; qblinks > 0; qblinks--) {
        quickBlinks();
    }
}

/* end. */
