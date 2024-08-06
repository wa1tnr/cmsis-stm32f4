/* main.c */
/* Tue  6 Aug 00:23:18 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

/****
 * PD15  LD6  Blue ***/

/* ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports */
/* RCC_APB2ENR  112, 146    GPIOx_CRH  172 */

#include <main.h>

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
