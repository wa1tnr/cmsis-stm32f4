/* main.c */
/* Wed 24 Jul 17:41:47 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

/****
 * PD15  LD6  Blue
 * PD12  LD4  Green  #  PD13 LD3  Orange  # PD14 LD5  Red
 ***/


#if 0
#include <stdint.h>
#endif


#include <stm32f4xx.h>
#include <stdbool.h>


#if 0
#include <stm32f4xx_hal_gpio_ex.h>
#define pinNumber 15 // PD15
#define SLOW_CONST 3
#endif

#include <delays.h>
#include <morse.h>

// ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports
// RCC_APB2ENR  112, 146    GPIOx_CRH  172

void blink() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void resetBlueLED() {
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void setBlueLED() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
}

void sendDit() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    ditDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
    ditDelayOffToOn();
}

void GPIOD_MODER_bang(void) {
    /* PD15 is Blue LED - all 4x on PORT D pins 12-15 */
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
}

#if 0
: TXE 80 ; \ USART_SR
: TC 40 ; \ USART_SR

  1000 0000 TXE
  0100 0000 TC

#endif

// return TXE set saying got shifted out
// TC is transmission complete and says that message was sent out the USART,
// and that TXE is set.
// happens '... and if TXE is set' so that is firmly implied (TC by itself
// may be enough; or perhaps TXE will be inspected here and there)

#if 0
: USART6_SR_TC?
  USART6_SR @ NOT
  TC AND
  0 = IF -1 EXIT THEN
  0 ;
#endif

bool olderUSART6_SR_STAT_TC(void) {
    uint32_t fetchedSR_TC_bit = USART6->SR & (1 << 6);
    if (fetchedSR_TC_bit == USART_SR_TC) {
        return 1;
    }
    return 0;
}

#if 0
bool USART6_SR_STAT_TC(void) {
        result = USART6_SR_TC_status();
        result = USART6_SR_TC_status();
#endif

bool USART6_SR_TC_status(void) {
    // uint32_t fetchedSR_TC_bit = USART6->SR & USART_SR_TC;
    // if (fetchedSR_TC_bit == USART_SR_TC) { return 1; }

    // if ((USART6->SR & USART_SR_TC) == (1 << 6)) {
    // if ((USART6->SR & USART_SR_TC) == 1) {
    if (
        (USART6->SR & USART_SR_TC) == 0
        ) {
        return 0;
    }
    return 1;
}

void usart_wait_send_ready(uint32_t usart) {
    /* Wait until the data has been transferred into the shift register. */
    // while ((USART_SR(usart) & USART_SR_TXE) == 0);
    // while ((USART_SR_TC(usart) & USART_SR_TXE) == 0);
}

extern void payloadCountedLoop(int count);

void do_the_thing(void) {
    payloadCountedLoop(3);
    for (int delayed = 2; delayed > 0; delayed--) {
        slowest();
    }
    payloadCountedLoop(8);
    for (int delayed = 200; delayed > 0; delayed--) {
        slowest();
    }
}


void turn_on_LED_forever() {
    GPIOD->BSRR = GPIO_BSRR_BR_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BS_15;
}

void turn_OUT_LED_forever() {
    GPIOD->BSRR = GPIO_BSRR_BS_15;
    blinkDelayOffToOn();
    GPIOD->BSRR = GPIO_BSRR_BR_15;
}

//  # 30.6.1 Status register USART_SR  p. 1007 incl TC (bit 6) and TXE (bit //  7)

void houseOfCardsFnOutCharUSART(char c) {
    // kills port D somehow
    bool result = 0;
    // FF AND USART6_DR !  outsent_TC? IF EXIT THEN
    USART6->DR = c & 0xFF; // bang DR
    while (!result) {
        result = USART6_SR_TC_status();
        // blink_once();
    }
    // blink_once();
    // blink_once();
}

#define oc(a) houseOfCardsFnOutCharUSART(a)

void initUSART6(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    GPIOC->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    GPIOC->AFR[0] |=
        (0x8UL << GPIO_AFRL_AFSEL6_Pos) | (0x8UL << GPIO_AFRL_AFSEL7_Pos);

    USART6->CR1 &= ~USART_CR1_UE;

    // USART6->CR1 |= USART_CR1_RE;
    // 0x8b for BRR if ting's reset clock
    // of iirc 8 MHz; otherwise follow
    // the source cited with 0x138 instead
    // USART6->BRR = 0x138;
    USART6->BRR = 0x8b; // 0x138;
    //                                          USART6->BRR = 0x138; // saw something at 115200 today
    // USART6->BRR = 0x8b;
    /* 115200 */ /* saw 0x8b or another value in .fs eforth source */
    USART6->CR1 |= USART_CR1_TE;
    USART6->CR1 |= USART_CR1_RE;

    /* ting does not bother with TE or RE in 2 lines above*/
    /* implemented locally in forth ..  */
    /* setting both _TE and _RE above */
    /* so keep that for now. */

    USART6->CR1 |= USART_CR1_UE; // TODO: unverified 18:38z-
    // saw this in ting's assembler, iirc.
    // not in ting's assembler.  Maybe in an example,
    // or in ting's PDF for hello world.
    // iirc the justification for the final _UE was that it
    // could not gain written bits without some kind
    // of disable or auto-disable and that this restores
    // a prior state, by ensuring it is 'now' enabled.
}

void enable_RCC_AHB1ENR_GPIOD() { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; }

// void sendDitsForever() { for (;;) { sendDit(); } }

void primary(void) {
    enable_RCC_AHB1ENR_GPIOD();
    GPIOD_MODER_bang();
    GPIOD->OTYPER = 0;
    GPIOD->OSPEEDR = 0;
    // sayHi();
}

void evenSlower() {
    for (int vySlow = 3; vySlow > 0; vySlow--) {
        slower();
    }
}

void testUSART6SendingAA() {
    for (int count = 144; count > 0; count--) {
        houseOfCardsFnOutCharUSART('a');
        // blink_once();
        // sendDitLongPause();
        houseOfCardsFnOutCharUSART('5');
        // blink_once();
        // sendDitLongPause();
    }
}

void quickBlinks() {
    for (int bln = 11; bln > 0; bln--) { blink_once(); }
}

int main(void) {
    primary();
    quickBlinks();

    initUSART6();
    for (;;) {
        testUSART6SendingAA();
        oc(' ');
        oc(' ');
        oc(' ');
        oc(' ');

        oc(' ');
        oc(' ');
        oc(' ');
        oc(' ');

        oc('S');
        oc('t');
        oc('a');
        oc('t');
        oc('i');
        oc('o');
        oc('n');
        oc(' ');
        oc('i');
        oc('d');
        oc('e');
        oc('n');
        oc('t');
        oc(' ');
        oc(' ');
        oc(' ');
        oc(' ');
        oc(' ');
        oc(' ');
        oc(' ');
        oc(' ');
        // blink_once();
    }


    while(-1);
    doLEDEarlyStuff();
    // turn_on_LED_forever();
    // while (-1) ;
    // turn_OUT_LED_forever();
    // houseOfCardsFnOutCharUSART('a');
    // turn_on_LED_forever();
    ldelayed();
    monitor();
    while (-1) ;
    // ldelayed();
    return 0;
}

/* end. */
