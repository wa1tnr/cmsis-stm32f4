## title here

This program does something pleasant.

* when make pio is typed, it compiles.
* when make erase is typed, it erases (the STM32F407 target board,
via USB cable connected to the usual USB-B port (the little one)).
* when make upload it typed, it uploads freshly-compiled firmware to stm32f407.
* when USART at PC6 (TX) is connected to CP2104 Friend (Adafruit) at RXD, it emits (sends a single character).
  In this way, full sentences are parsed from a string constant, into a character buffer ('char buffer[32]'), using snprintf and sizeof arg, and later read out as single printed chars and sent to USART6.

Interesting: this editor is handling use of parens! (as when we try to fail (foo bar) something).

#### Sat  3 Aug 18:13:17 UTC 2024

#### end.
