/* morse.h */
/* Wed 24 Jul 17:41:47 UTC 2024 */

#if 0
// unused externs:
extern void resetBlueLED();
extern void setBlueLED();
extern void sendDit();
extern void sendDitsForever();
extern void sendDitLongPause();
extern void sendDahLongPause();
extern void sendMorseLtrR();
extern void threeDits();
extern void threeDahs();
extern void morseSpace();
#endif

extern void sendDitLongPause();

// at this build only one extern function required:
extern void doLEDEarlyStuff();

/* end. */
