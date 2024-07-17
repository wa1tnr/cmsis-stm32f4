
/****
 *
 * Wed 17 Jul 05:51:07 UTC 2024
 *
 * STATUS - although the code was guesswork, at a minimum,
 * it is operational (the blue LED continues to blink after
 * the 'fiasco' section of the code WRT USART6).
 *
 *    monitor() is now called after the USART6 routines (the
 *    function, largest_USART6() .. which calls all else wrt
 *    USART6 setups).
 *
 */

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
#include <stm32f4xx_hal_gpio_ex.h>

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


#if 0
  SET_GPIOC_MODER_PC6_PC7_ALT_A
  SET_AF8_BITS_GPIOC_AFRL // sets AF8 for PC6, PC7
  SETUP_USART6_CR1 // UE set
  SET_USART6_BRR
#endif

void GPIOC_MODER_bang(void) {
    // ( n -- )
    // GPIOC -> MODER |= GPIO_MODER_MODER7_0;
    // GPIOC -> MODER |= GPIO_MODER_MODER6_0;
    GPIOC->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7;
}

#if 0
: GPIOC_MODER! ( n -- )
  GPIOC_MODER @
  or GPIOC_MODER ! ;
#endif

#if 0
void AF_MODE(void) {
: AF_MODE ( n -- ) \ modeled on the OUTPUT word
  \ F000 \ mask for PC6/PC7  T/X  PC6 is TX confirmed 17 July 2024 02:27z
  6 max 7 min \ restrict to pins 6 and 7
  2 * 1 + 1
  swap <<
  GPIOC_MODER! ;
}
#endif

void SET_GPIOC_MODER_PC6_PC7_ALT_A(){
    // GPIOC -> AFR[0] |= (GPIO_AF8_USART6  << GPIO_AFRL_AFRL6_3) |
    //   GPIO_AF8_USART6 << GPIO_AFRL_AFRL7_3;

    GPIOC -> AFR[0] |=
    (0x8UL << GPIO_AFRL_AFSEL6_Pos) |
    (0x8UL << GPIO_AFRL_AFSEL7_Pos);

    // empty and below: SET_AF8_BITS_GPIOC_AFRL();

    // GPIOC -> AFR |= GPIO_AFRL_AFSEL6_3;
    // GPIOC -> AFR |= GPIO_AFRL_AFSEL7_3;
    // so is the AFR 3 the USART or what
    // GPIOC_MODER_bang();
}

#if 0
: SET_GPIOC_MODER_PC6_PC7_ALT_A ( -- )
  6 AF_MODE
  7 AF_MODE ;
#endif

void nAF8(){

}
#if 0
: nAF8 3 + ; ( n -- n+3 )
#endif

void AF8_BITS() { // 0x0800 0000
    // AF8 is for USART6 and others
    // 6 AF8_BITS  0x0800 0000
    //    1111 1111 1111 1111 1111 1111 1111 1111
    //    1098 7654 3210 9876 5432 1098 7654 3210
    //    0000 1000 0000 0000 0000 0000 0000 0000
}
// #define GPIO_AFRL_AFSEL6_3               (0x8UL << GPIO_AFRL_AFSEL6_Pos) /*!<
// 0x08000000 */
#if 0
\ USART6 needs AF8 not AF7
: AF8_BITS ( n -- )
  \ as OUTPUT word is structured
  6 max 7 min
  4 *
  nAF8 1 swap << ; \ 0x88foo
#endif

void SET_AF8_BITS_GPIOC_AFRL(){
    // GPIOC->MODER |= GPIO_MODER_MODER15_0;
    // sets AF8 for PC6, PC7
}
#if 0

: SET_AF8_BITS_GPIOC_AFRL
  6 AF8_BITS    \ 0x0800 0000
  7 AF8_BITS    \ 0x8000 0000
  or            \ 0x8800 0000
  GPIOC_AFRL! ;

#endif

void SETUP_USART6_CR1(void) {
    // UE set
    USART6->CR1 &= ~USART_CR1_UE;
    USART6->CR1 |= USART_CR1_RE;
    USART6->BRR = 0x138; // 115200
    // USART6 -> CR3 |= USART_CR3_DMAR;  // no this is for dma
    USART6->CR1 |= USART_CR1_UE; // enable USART6
}

void largest_USART6(void) {
    SIO_RCC_bang();
    SET_GPIOC_MODER_PC6_PC7_ALT_A();
    GPIOC_MODER_bang();
    SETUP_USART6_CR1();
    }

int main(void) {
    primary();
    largest_USART6();
    monitor();
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

// https://wenku.csdn.net/answer/5856daca27c64c94ad9dab8298e040f3
//   GPIOC -> MODER  |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
//   GPIOC -> AFR[0] |= GPIO_AF8_USART6   << GPIO_AFRL_AFRL6_[pos] |
//   GPIO_AF8_USART6 << GPIO_AFRL_AFRL7_[pos];
// wild guess: GPIO_AFRL_AFRL6_3


//  TODO: find
//        GPIO_AF8_USART6


// $ cat -n /some/path/to/stm32f41xcmsis/stm32f4xx_hal_gpio_ex.h | ag 347
//   347 #define GPIO_AF8_USART6        ((uint8_t)0x08)  /* USART6 Alternate Function mapping */


// end.
