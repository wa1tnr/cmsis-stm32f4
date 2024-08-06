/* morse.c */
/* Tue  6 Aug 00:23:18 UTC 2024 */

#if 0
#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio_ex.h>
#endif
#include <delays.h>
#include <main.h>

// #include <main.h>
// #define SLOW_CONST 3
// const int baseN = 55000;
// const int N = baseN * 4;

// void resetBlueLED() { GPIOD->BSRR = GPIO_BSRR_BR_15; }
// void setBlueLED() { GPIOD->BSRR = GPIO_BSRR_BS_15; }

// void sendDit(); // main.c
// void sendDit() { GPIOD->BSRR = GPIO_BSRR_BS_15; ditDelayOffToOn(); GPIOD->BSRR = GPIO_BSRR_BR_15; ditDelayOffToOn(); }

void sendDitsForever() {
    for (;;) {
        sendDit();
    }
}

void sendDitLongPause() {
    setBlueLED();
    slowing();
    resetBlueLED();
    for (int thisSlow = 3; thisSlow > 0; thisSlow--) {
        slowing();
    }
}

void sendDahLongPause() {
    setBlueLED();
    slower();
    slowing(); // fine tune dah length

    resetBlueLED();
    for (int thisSlow = 3; thisSlow > 0; thisSlow--) {
        slowing();
    }
}

void sendMorseLtrR() {
    sendDitLongPause();
    sendDahLongPause();
    sendDitLongPause();
}

void sendMorseLtrN() {
    sendDahLongPause();
    sendDitLongPause();
}

void sendMorseLtrO() {
    for (int count = 3; count > 0; count--) {
        sendDahLongPause();
    }
}

void threeDits() {
    for (int dits = 3; dits > 0; dits--) {
        sendDitLongPause();
    }

}

void threeDahs() {
    for (int dits = 3; dits > 0; dits--) {
        sendDahLongPause();
    }
}

void sendMorseSpace() {
    for (int thisSlow = 9; thisSlow > 0; thisSlow--) {
        slowing();
    }
}

void sendMorseWSpace() {
    // between 'words' such as 'hello'
    for (int iterations = 3; iterations > 0; iterations--) {
        sendMorseSpace(); // want a fractional like 3.2 here ;)
    }
}

void lnthyWSpaceIval() {
    for (int lwSpace = 2; lwSpace > 0; lwSpace--) {
        sendMorseWSpace();
    }
}

void doLEDEarlyStuff() {
    resetBlueLED();

    threeDits();
    sendMorseSpace();
    threeDahs();
    sendMorseSpace();
    threeDits();
    sendMorseSpace();


    sendMorseSpace();
    sendMorseSpace();
    sendMorseSpace();
    sendMorseSpace();

    sendMorseLtrR();
    sendMorseSpace();
    sendMorseLtrR();
    sendMorseSpace();
    sendMorseLtrR();
    sendMorseSpace();

    sendMorseSpace();
    sendMorseSpace();
    sendMorseSpace();
    sendMorseSpace();
    sendMorseSpace();

    sendMorseLtrR();
    sendMorseSpace();

    monitor();
    sendDitsForever();
    while (-1) ;
}

/* end. */
