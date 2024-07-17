/* Wed 17 Jul 00:32:54 UTC 2024 */

/***
 *
 * PD15  LD6  Blue
 *
 * PD12  LD4  Green  #  PD13 LD3  Orange  # PD14 LD5  Red
 *
 ***/

/***
 *
 * NEWS - blue LED stm32f407 Discovery now blinks blue LED.
 *
 ***/

#include <stdbool.h>
#include <stdint.h>
#include <stm32f4xx.h>

#define pinNumber 15 // PD15
#define SLOW_CONST 5
#define PIPS 21 // 5

// ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports
// RCC_APB2ENR  112, 146    GPIOx_CRH  172

// const int N = 500000;
const int N = 212345;

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

void blink_once() {
    GPIOD->BSRR = GPIO_BSRR_BS_15; // SET is ON forever - Blue LED
    for (volatile int i = N; i > 0; i--)
        ;
    GPIOD->BSRR = GPIO_BSRR_BR_15;
    slower();
    // for (volatile int i = OTIME; i > 0; i--) ;
}

void payload(void) {
    for (int count = PIPS; count > 0; count--) {
        blink_once();
    }
    for (int delays = 3; delays > 0; delays--) {
        slower();
    }
}

void payloadThreeX() {
    for (int iter = 2; iter > 0; iter--) {
        payload();
    }
}

void monitor(void) {
    for (;;) {
        // payload();
        blink_once();
        for (int j = 2; j > 0; j--) {
            slowest();
        }
    }
}

void forever(void) {
    for (;;) {
        payload();
    }
}

void primary(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
    GPIOD->OTYPER = 0;
    GPIOD->OSPEEDR = 0;
    payloadThreeX();
    // forever();
}

void SIO_RCC_bang(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
}

void GPIOC_MODER_bang(void) {
    // ( n -- )
    GPIOC->MODER |= GPIO_MODER_MODER6_0;
    GPIOC->MODER |= GPIO_MODER_MODER7_0;
}

#if 0
: GPIOC_MODER! ( n -- )
  GPIOC_MODER @
  or GPIOC_MODER ! ;
#endif

#if 0
void AF_MODE(void) {
: AF_MODE ( n -- ) \ modeled on the OUTPUT word
  \ F000 \ mask for PC6/PC7
  6 max 7 min \ restrict to pins 6 and 7
  2 * 1 + 1
  swap <<
  GPIOC_MODER! ;
}
#endif

void SET_GPIOC_MODER_PC6_PC7_ALT_A() { GPIOC_MODER_bang(); }

#if 0
: SET_GPIOC_MODER_PC6_PC7_ALT_A ( -- )
  6 AF_MODE
  7 AF_MODE ;
#endif

void largest_USART6(void) { SIO_RCC_bang(); }

int main(void) {
    primary();
    monitor();
    largest_USART6();
    return 0;
}

/* end. */

#if 0

: SIO_RCC! ( -- ) ( verif SED )
  RCC_AHB1ENR @
  GPIOCEN or
  RCC_AHB1ENR !

  RCC_APB2ENR @
  USART6EN or
  RCC_APB2ENR ! ;

: SETUP_USART6 \ main entry into Lumex support
  SIO_RCC!
  SET_GPIOC_MODER_PC6_PC7_ALT_A
  SET_AF8_BITS_GPIOC_AFRL \ sets AF8 for PC6, PC7
  SETUP_USART6_CR1 \ UE set
  SET_USART6_BRR
  \ setup USART6 baud rate mantissa and fraction
  SETUP_USART6_CR1_TE ;
  \ setup USART6_CR1 TE and RE - UE done previously
#endif
