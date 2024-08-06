/* usart.c */
/* Mon  5 Aug 01:36:14 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

#include <delays.h>
#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>

#define printBuffer(a) printBufferToUSART6(a)
#define oc(a) outputCharUSART6(a)
#define ASCII_LF 0x0a

char buffer[32];

/* 30.6.1 Status register USART_SR  p. 1007 incl TC (bit 6) and TXE (bit 7) */
void outputCharUSART6(char c) {
    bool result = 0;
    while (!result) {
        result = ((USART6->SR & USART_SR_TXE) == USART_SR_TXE);
    }
    USART6->DR = c & 0xFF;
    result = 0;
    while (!result) {
        result = ((USART6->SR & USART_SR_TC) == USART_SR_TC);
    }
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

void printBufferToUSART6Old() {
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

void printBufferToUSART6(char *inputBuffer) {
    while (*inputBuffer) {
        if (*inputBuffer == '\0') {
            return;
        }
        if (*inputBuffer == ASCII_LF) {
            oc('\n');
            return;
        }
        if (*inputBuffer > (char)0x1F) {
            oc(*inputBuffer++);
        }
    }
}

void printLF() {
    snprintf(buffer, sizeof buffer, "%c", '\n');
    printBuffer(buffer);
}

void printTestMessage() {

    snprintf(buffer, sizeof buffer, "%s", " printBuffer(buffer)");
    printBuffer(buffer);

    snprintf(buffer, sizeof buffer, "%s", " in use: ");
    printBuffer(buffer);

    snprintf(buffer, sizeof buffer, "%s", " telnavika  01:35z");
    printBuffer(buffer);

    printLF();
}

/* end. */
