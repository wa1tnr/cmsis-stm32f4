/* main.c */
/* Mon 12 Aug 18:29:44 UTC 2024 */

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
    lnthyWSpaceIval();
    ldelayed();
    monitor();
    while (-1)
        ;
    return 1;
}

/* end. */
