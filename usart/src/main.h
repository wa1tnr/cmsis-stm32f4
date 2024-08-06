/* main.h */
/* Tue  6 Aug 00:23:18 UTC 2024 */

#ifndef MAIN_HEADER_FILE
#define MAIN_HEADER_FILE

#include <delays.h>
#include <morse.h>
#include <stdbool.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <usart.h>
#include <gpio.h>

#define oc(a) outputCharUSART6(a)
#define ASCII_CR 0x0d
#define ASCII_LF 0x0a
#endif

/* end. */
