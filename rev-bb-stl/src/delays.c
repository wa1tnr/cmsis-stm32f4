
/****
 *
 * Wed 17 Jul 05:51:07 UTC 2024
 *
 */

// #include <Arduino.h>
// #include <stdbool.h>
// #include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio_ex.h>

// #define pinNumber 15 // PD15
#define SLOW_CONST 3

const int nN = 55000;
const int N = nN * 4;

void slowing() {
    for (volatile int i = N; i > 0; i--)
        ;
}

void slower() {
    for (volatile int lp = SLOW_CONST; lp > 0; lp--) {
        slowing();
    }
}

void slowest() {
    for (volatile int lp = SLOW_CONST; lp > 0; lp--) {
        slower();
    }
}

void ldelayed() {
    for (volatile int lp = SLOW_CONST; lp > 0; lp--) {
        slowest();
    }
}

void blink_once() {
    GPIOD->BSRR = GPIO_BSRR_BS_15; // SET is ON forever - Blue LED
    for (volatile int i = nN; i > 0; i--)
        ;
    GPIOD->BSRR = GPIO_BSRR_BR_15;
    slowing();
    // for (volatile int i = OTIME; i > 0; i--) ;
}

void payload(void) {
    blink_once();
    for (int delays = 3; delays > 0; delays--) {
        slowing();
    }
}

void payloadCountedLoop(int count) {
    for (int iter = count; iter > 0; iter--) {
        payload();
    }
}

void sayHi() {
    payloadCountedLoop(3);
    for (int delayed = 2; delayed > 0; delayed--) {
        slowest();
    }

    payloadCountedLoop(2);
    for (int delayed = 2; delayed > 0; delayed--) {
        slowest();
    }

    payloadCountedLoop(3);

    for (int delayed = (6 * 3); delayed > 0; delayed--) {
        slowest();
    }
}

void monitor(void) {
    for (;;) {
        blink_once();
        for (int j = 7; j > 0; j--) { // was 2, and 3 not enough delay 0055z thu
            slowest();
        }
    }
}

#if 0
void payload(void) {
    blink_once();
    for (int delays = 3; delays > 0; delays--) {
        slowing();
    }
}

void payloadCountedLoop(int count) {
    for (int iter = count; iter > 0; iter--) {
        payload();
    }
}

#endif

/* end. */