/* usart.c */
/* Mon 12 Aug 18:29:44 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <buffer.h>

#define printBuffer(a) printBufferToUSART6(a)

#define oc(a) outputCharUSART6(a)
#define ASCII_LF 0x0a

/* char buffer[32]; */
/* CANON char buffer[64]; */

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

#define TIMESTAMP "Sat Aug 17 00:59:15 UTC 2024"

void printTestMessageText() {
    snprintf(buffer, sizeof buffer, "%s", " printBuffer(buffer)");
    printBuffer(buffer);

    snprintf(buffer, sizeof buffer, "%s", " in use: ");
    printBuffer(buffer);
    printLF();

    snprintf(buffer, sizeof buffer, "%c%s", ' ', TIMESTAMP);
    printBuffer(buffer);
    printLF();

    snprintf(buffer, sizeof buffer, "%s%c", " Plano, TX  00:59z  tupelo  Cincinatti, OH", ' ');
    printBuffer(buffer);

    /***
     *
     * 64:
     * 1234567890123456789012345678901234567890123456789012345678901234
     *
     * 32:
     * 12345678901234567890123456789012
     *
     */
}

void printTestMessage() {
    printTestMessageText();
    printLF();
    printLF();
    printTestMessageText();
}

/* end. */
