/* delays.c */
/* Mon 29 Jul 19:20:53 UTC 2024 */

#if 0
#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio_ex.h>
#endif

const int baseN = 55000;
const int N = baseN * 4;
#define SLOW_CONST 3
#include <main.h>

void blinkDelayOffToOn() {
    for (volatile int i = baseN; i > 0; i--)
        ;
}

void ditDelayOffToOn() {
    for (volatile int i = (baseN * 128); i > 0; i--) {
        blinkDelayOffToOn();
    }
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

void evenSlower() {
    for (int vySlow = 3; vySlow > 0; vySlow--) {
        slower();
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
        for (int j = 4; j > 0; j--) { // 7 nice and long; 4 during tests.  was
                                      // 2, and 3 not enough delay 0055z thu
            slowest();
        }
    }
}

/* end. */
