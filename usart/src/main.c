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
#include <usart.h>

/* ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports */
/* RCC_APB2ENR  112, 146    GPIOx_CRH  172 */

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
