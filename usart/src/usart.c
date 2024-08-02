/* main.c */
/* Fri  2 Aug 20:46:28 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

/****
 * PD15  LD6  Blue ***/

#include <delays.h>
#include <morse.h>
#include <stdbool.h>
#include <stdio.h>
#include <stm32f4xx.h>

/* ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports */
/* RCC_APB2ENR  112, 146    GPIOx_CRH  172 */

#define oc(a) outputCharUSART6(a)
#define ASCII_CR 0x0d
#define ASCII_LF 0x0a

char buffer[32];

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

/* 30.6.1 Status register USART_SR  p. 1007 incl TC (bit 6) and TXE (bit 7) */
void outputCharUSART6(char c) {
    bool result = 0;
    // TODO: avoid 'while' loops ENTIRELY:
    while (!result) {
        result = ((USART6->SR & USART_SR_TXE) == USART_SR_TXE);
    }
    USART6->DR = c & 0xFF;
    result = 0;
    while (!result) {
        result = ((USART6->SR & USART_SR_TC) == USART_SR_TC);
    }
}

void initGPIO(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
    GPIOD->OTYPER = 0;
    GPIOD->OSPEEDR = 0;
}

/* 0x8b for BRR if using C.H. Ting's reset clock of iirc 8 MHz */
void initUSART6(void) {
    bool result = 0;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    GPIOC->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    GPIOC->AFR[0] |=
        (0x8UL << GPIO_AFRL_AFSEL6_Pos) | (0x8UL << GPIO_AFRL_AFSEL7_Pos);
    USART6->CR1 &= ~USART_CR1_UE;
    USART6->BRR = 0x8b; // 0x138;
    while (!result) {
        result = ((USART6->SR & USART_SR_TXE) == USART_SR_TXE);
    }
    USART6->CR1 |= USART_CR1_TE;
    USART6->CR1 |= USART_CR1_RE;
    USART6->CR1 |= USART_CR1_UE;
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

void printBufferToUSART6() {
    int bufCharCount = (sizeof buffer) / (sizeof buffer[0]);
    for (int arrayIndex = 0; arrayIndex < bufCharCount; arrayIndex++) {
        if (buffer[arrayIndex] == '\0') {
            return; // filter for nulls or garbage after
        }
        if (buffer[arrayIndex] == ASCII_LF) {
            oc('\n');
            return;
        }
        if (buffer[arrayIndex] > (char)0x1F) {
            oc(buffer[arrayIndex]);
        }
    }
}

void printLF() {
    snprintf(buffer, sizeof buffer, "%c", '\n');
    printBufferToUSART6();
}

void printTestMessage() {
    // snprintf(buffer, sizeof buffer, "%s", "How are they doing now?");
    snprintf(buffer, sizeof buffer, "%s", " NEW usart directory 19:47z");
    printBufferToUSART6();
    printLF();
    for (int qblinks = 3; qblinks > 0; qblinks--) {
        quickBlinks();
    }
}

void lnthyWSpaceIval() {
    for (int lwSpace = 2; lwSpace > 0; lwSpace--) {
        sendMorseWSpace();
    }
}

int main(void) {
    initGPIO();
    quickBlinks();
    initUSART6();
    printTestMessage();
    printTestMessage();
    lnthyWSpaceIval();
    ldelayed();
    monitor();
    while (-1)
        ;
    doLEDEarlyStuff();
    while (-1) {
        quickBlinks();
    }
    return 0;
}

/* end. */
