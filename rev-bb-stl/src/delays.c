
/****
 *
 * Thu 18 Jul 19:51:50 UTC 2024
 *
 */

#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio_ex.h>

#define SLOW_CONST 3

const int nN = 55000;
const int N = nN * 4;

void blinkDelayOffToOn() {
    for (volatile int i = nN; i > 0; i--) ;
}

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

extern void blink();

void blink_once() {
    blink();
    slowing();
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

/* end. */
