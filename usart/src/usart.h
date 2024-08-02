/* usart.h */
/* Fri  2 Aug 20:56:43 UTC 2024 */

// #include <delays.h>
// #include <morse.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stm32f4xx.h>

// #define oc(a) outputCharUSART6(a)
// #define ASCII_CR 0x0d
// #define ASCII_LF 0x0a

#if 0
extern char buffer[32];
extern void resetBlueLED();
extern void setBlueLED();
extern void sendDit();
extern void GPIOD_MODER_bang(void);
extern void turn_on_LED_forever();
extern void turn_OUT_LED_forever();
extern void outputCharUSART6(char c);
#endif

extern void initGPIO(void);
extern void quickBlinks();
extern void initUSART6(void);
extern void printTestMessage();
extern void lnthyWSpaceIval();
// extern void blink();
// extern void sendMorseMsgNO();
// extern void printBufferToUSART6();
// extern void printLF();

/* end. */
