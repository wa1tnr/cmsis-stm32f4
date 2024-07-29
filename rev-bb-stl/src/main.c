/* main.c */
/* Mon 29 Jul 19:20:53 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

/****
 * PD15  LD6  Blue
 ***/

#include <delays.h>
#include <morse.h>
#include <stdbool.h>
#include <stdio.h>
#include <stm32f4xx.h>

/* ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports */
/* RCC_APB2ENR  112, 146    GPIOx_CRH  172 */

void blink() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void resetBlueLED() { GPIOD->BSRR = GPIO_BSRR_BR_15; }

void setBlueLED() { GPIOD->BSRR = GPIO_BSRR_BS_15; }

void sendDit() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    ditDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
    ditDelayOffToOn();
}

void GPIOD_MODER_bang(void) {
    /* PD15 is Blue LED - all 4x on PORT D pins 12-15 */
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
}

/***
 *
 * return TXE set saying got shifted out
 *
 * TC is transmission complete and says that message
 * was sent out the USART, and that TXE is set.
 * [garbled; lost text here] .. happens
 * '... and if TXE is set' so that is firmly implied
 * (TC by itself may be enough;
 * or perhaps TXE will be inspected here and there)
 *
 ***/

bool USART6_SR_TC_status(void) {
    if ((USART6->SR & USART_SR_TC) == 0) {
        return 0;
    }
    return 1;
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

/***
 *
 * 30.6.1 Status register USART_SR  p. 1007 incl TC (bit 6) and TXE (bit 7)
 *
 ***/

void outputCharUSART6(char c) {
    bool result = 0;
    USART6->DR = c & 0xFF; // bang DR
    while (!result) {
        result = USART6_SR_TC_status();
    }
}
/****
 *
 * 0x8b for BRR if using C.H. Ting's reset clock of iirc 8 MHz
 * otherwise follow the source cited with 0x138 instead.
 *
 *     USART6->BRR = 0x138;
 *
 ***/

void initUSART6(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    GPIOC->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    GPIOC->AFR[0] |=
        (0x8UL << GPIO_AFRL_AFSEL6_Pos) | (0x8UL << GPIO_AFRL_AFSEL7_Pos);
    USART6->CR1 &= ~USART_CR1_UE;

    USART6->BRR = 0x8b; // 0x138;
    USART6->CR1 |= USART_CR1_TE;
    USART6->CR1 |= USART_CR1_RE;
    USART6->CR1 |= USART_CR1_UE;
}

void enable_RCC_AHB1ENR_GPIOD() { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; }

void primary(void) {
    enable_RCC_AHB1ENR_GPIOD();
    GPIOD_MODER_bang();
    GPIOD->OTYPER = 0;
    GPIOD->OSPEEDR = 0;
}

void quickBlinks() {
    for (int bln = 3; bln > 0; bln--) {
        blink_once();
    }
}

#define oc(a) outputCharUSART6(a)

void sendMorseMsgNO() {
    sendMorseLtrN();
    sendMorseSpace();
    sendMorseLtrO();
    sendMorseSpace();
    sendMorseWSpace();
}

char buffer[32];

void printBufferToUSART6() {
    int bufCharCount = (sizeof buffer) / (sizeof buffer[0]);

    for (int arrayIndex = 0; arrayIndex < bufCharCount; arrayIndex++) {

        if (buffer[arrayIndex] == '\0') {
            return; // filter for nulls or garbage after
        }

        if (buffer[arrayIndex] > (char)0x1F) {
            oc(buffer[arrayIndex]);
        }
    }
}

/* not really a storage op: */
void storeMessage() {
    /* buy some time prior to picocom USART serial connection */
    for (int sent = 5; sent > 0; sent--) {
        sendMorseWSpace();
    }

    /* system seems to eat the first char sent so add one extra: */
    snprintf(buffer, sizeof buffer, "%c", '\n');
    printBufferToUSART6();

    /* 24 char message: */
    snprintf(buffer, sizeof buffer, "%s", "How are they doing now?");
    /*                                     123456789012345678901234 */

    printBufferToUSART6();

    /* Old test code: */
    if (buffer[7] == 'j') {
        sendMorseMsgNO();
        sendMorseMsgNO();
    }

    /* continue active code: */
    for (int sent = 2; sent > 0; sent--) {
        sendMorseWSpace();
    }
    for (int qblinks = 10; qblinks > 0; qblinks--) {
        quickBlinks();
    }
    sendMorseWSpace();
}

void lnthyWSpaceIval() {
    for (int lwSpace = 5; lwSpace > 0; lwSpace--) {
        sendMorseWSpace();
    }
}

int main(void) {
    primary();
    quickBlinks();
    sendMorseSpace();
    initUSART6();
    storeMessage();
    lnthyWSpaceIval();
    while (-1) {
        quickBlinks();
    }

    /* blocked from exec: */
    doLEDEarlyStuff();
    ldelayed();
    monitor();
    while (-1)
        ;
    return 0;
}

/* end. */
