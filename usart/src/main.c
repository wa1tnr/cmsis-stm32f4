/* main.c */
/* Sat  3 Aug 16:48:19 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

/****
 * PD15  LD6  Blue ***/

/* ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports */
/* RCC_APB2ENR  112, 146    GPIOx_CRH  172 */
#include <main.h>
#define oc(a) outputCharUSART6(a)
#define ASCII_CR 0x0d
#define ASCII_LF 0x0a

/* 30.6.1 Status register USART_SR  p. 1007 incl TC (bit 6) and TXE (bit 7) */
/* 0x8b for BRR if using C.H. Ting's reset clock of iirc 8 MHz */

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
