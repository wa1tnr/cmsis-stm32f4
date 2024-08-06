/* gpio.h */
/* Tue  6 Aug 00:23:18 UTC 2024 */

#ifndef GPIO_HEADER_FILE
#define GPIO_HEADER_FILE
extern void resetBlueLED();
extern void setBlueLED();
extern void initGPIO(void);
extern void quickBlinks();
extern void blink();
extern void sendDit();
#endif

/* end. */
