/* main.c */
/* Wed 24 Jul 17:41:47 UTC 2024 */

/* USART6 enable and write-only (no listener) */
/* port:  Forth source to C language */

/* utilise: CMSIS notation and paradigm */

/****
 * PD15  LD6  Blue
 ***/

#include <stdbool.h>
#include <stm32f4xx.h>
#include <delays.h>
#include <morse.h>
#include <stdio.h>

// ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports
// RCC_APB2ENR  112, 146    GPIOx_CRH  172

// extern void payloadCountedLoop(int count);
// extern void sendMorseLtrN();
// extern void sendMorseLtrO();

void blink() {
  GPIOD->BSRR = GPIO_BSRR_BS_15;
  blinkDelayOffToOn();
  GPIOD->BSRR = GPIO_BSRR_BR_15;
}

void resetBlueLED() { GPIOD->BSRR = GPIO_BSRR_BR_15; }

void setBlueLED() { GPIOD->BSRR = GPIO_BSRR_BS_15; }

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

bool USART6_SR_TC_status(void) {
  if ((USART6->SR & USART_SR_TC) == 0) {
    return 0;
  }
  return 1;
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

//  # 30.6.1 Status register USART_SR  p. 1007 incl TC (bit 6) and TXE (bit //
//  7)

void houseOfCardsFnOutCharUSART(char c) {
  bool result = 0;
  // FF AND USART6_DR !  outsent_TC? IF EXIT THEN
  USART6->DR = c & 0xFF; // bang DR
  while (!result) {
    result = USART6_SR_TC_status();
  }
}

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
  //                                          USART6->BRR = 0x138; // saw
  //                                          something at 115200 today
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

void primary(void) {
  enable_RCC_AHB1ENR_GPIOD();
  GPIOD_MODER_bang();
  GPIOD->OTYPER = 0;
  GPIOD->OSPEEDR = 0;
}

void evenSlower() {
  for (int vySlow = 3; vySlow > 0; vySlow--) {
    slower();
  }
}

void testUSART6SendingAA() {
  for (int count = 36; count > 0; count--) {
    houseOfCardsFnOutCharUSART('a');
    houseOfCardsFnOutCharUSART('5');
  }
}

void testUSART6SendingBB() {
  for (int count = 4; count > 0; count--) {
    houseOfCardsFnOutCharUSART('a');
    houseOfCardsFnOutCharUSART('5');
  }
}

void quickBlinks() {
  for (int bln = 3; bln > 0; bln--) {
    blink_once();
  }
}

#define oc(a) houseOfCardsFnOutCharUSART(a)

void trillBlink(void) {
  for (int blinks = 12; blinks > 0; blinks--) {
    blink_once();
  }
}

void sendMorseMsgNO() {
  sendMorseLtrN();
  sendMorseSpace();
  sendMorseLtrO();
  sendMorseSpace();
  // for (int bigSpaceCount = 4; bigSpaceCount > 0; bigSpaceCount--) {
  sendMorseWSpace();
  // }
}

#if 0
const char message[8] = {'c', '0', 'p', 'h', 'e', 'a', 'r', 'j'};
#endif

char buffer[32];

void printBufferToUSART6() {
  int bufCharCount = (sizeof buffer) / (sizeof buffer[0]);

  for (int thaang = 0; thaang < bufCharCount; thaang++) {

    if (buffer[thaang] == '\0') {
      return; //early; do not process nulls nor stay in the loop after
    }

    if (buffer[thaang] > (char) 0x1F) {
      oc(buffer[thaang]);
    }
  }
}

void storeMessage() {
  // buy some time to establish USART serial conversation with picocom
  sendMorseWSpace();
  sendMorseWSpace();
  sendMorseWSpace();
  sendMorseWSpace();
  sendMorseWSpace();

  // 24 char message:

  snprintf(buffer, sizeof buffer, "%s", "How are they doing now?");
  //                                     123456789012345678901234


  // NOTE: factor to a function so that it can return early when
  // encountering a NULL at the end of the string residing in buffer[]:
  printBufferToUSART6();

#if 0
  for (int thaang = 0; thaang < bufCharCount; thaang++) {
    // if (buffer[thaang] > '0') { // think of a better test perhaps
    if (buffer[thaang] > (char) 0x1F) { // cast to char to be safe maybe
      oc(buffer[thaang]);
    }
  }
#endif

  // RESULT: YES THE PRINTING WORKS FINE.
  // NOTE: the test against '0' did not catch ascii 32 (0x20)

  if (buffer[7] == 'j') {
    sendMorseMsgNO();
    sendMorseMsgNO();
  }

  sendMorseWSpace();
  sendMorseWSpace();
  quickBlinks();
  quickBlinks();
  quickBlinks();
  quickBlinks();
  quickBlinks();
  quickBlinks();
  quickBlinks();
  quickBlinks();
  quickBlinks();
  quickBlinks();

  sendMorseWSpace();
}

void sendMessage() {
  // buffer
}

int main(void) {
  primary();
  quickBlinks();
  sendMorseSpace();
  initUSART6();
  storeMessage();
  sendMorseWSpace();
  sendMorseWSpace();
  sendMorseWSpace();
  sendMorseWSpace();
  sendMorseWSpace();
  while(-1) {
    quickBlinks();
  }

  for (;;) {
    testUSART6SendingAA();
    testUSART6SendingBB();
    slowest();
  }

  while (-1)
    ;
  doLEDEarlyStuff();
  ldelayed();
  monitor();
  while (-1)
    ;
  return 0;
}

/****
 *
 * TODO: dreams
 * TODONE: wow. ;) was easy enough and original approach worked great.
 *
 * 28 July 2024, 13:34z
 *
 * consolidate how 'printing' to the USART works, so
 * that it begins to resemble a handler that accepts
 * a text string (char array) as 'input'.
 *
 * Should pretty-up the code some (make it a bit less
 * gangly).
 *
 */

/* end. */
