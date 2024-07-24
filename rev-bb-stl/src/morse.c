/* morse.c */
/* Wed 24 Jul 17:41:47 UTC 2024 */

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

void morseSpace() {
    for (int thisSlow = 9; thisSlow > 0; thisSlow--) {
        slowing();
    }
}

void doLEDEarlyStuff() {
    resetBlueLED();

    threeDits();
    morseSpace();
    threeDahs();
    morseSpace();
    threeDits();
    morseSpace();


    morseSpace();
    morseSpace();
    morseSpace();
    morseSpace();

    sendMorseLtrR();
    morseSpace();
    sendMorseLtrR();
    morseSpace();
    sendMorseLtrR();
    morseSpace();

    morseSpace();
    morseSpace();
    morseSpace();
    morseSpace();
    morseSpace();

    sendMorseLtrR();
    morseSpace();

    monitor();
    sendDitsForever();
    while (-1) ;
}

// int main(void) {
//    doLEDEarlyStuff();
// }

/* end. */
