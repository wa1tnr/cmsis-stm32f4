#!/bin/sh
# Sat Aug 17 12:41:54 AM UTC 2024

# reset and upload to stm32f407 Discovery (or most stm32 boards)

# the --reset parm:
#
# st-flash --reset write ${FIRMW} ${ADDR}
#    .. returns nothing - it seems to make a difference, perhaps on
#    the target itself .. tested earlier and found to be required
#    for a more nuanced result (do not remember atm what that was).
#
#    Not essential; just desired.

# KNOWN ISSUE
#   [ https://github.com/stlink-org/stlink/issues/220 ]
#   solved - unstrap BOOT0 and DONE.

#   narrative - found stm32f407 Discovery board had the BOOT0
#       jumper strapped.  This prevented desired operation.

FIRMW=./.pio/build/disco_f407vg/firmware.bin
ADDR=0x08000000

erasing() {
	st-flash erase
}

writing_no_reset() {
	st-flash write ${FIRMW} ${ADDR}
}

writing() {
	st-flash --reset write ${FIRMW} ${ADDR}
}

echoes_floyd() {
	echo;echo;date;echo
}

# - - -   m a i n   - - -

# erasing
echoes_floyd
# writing_no_reset
writing
echoes_floyd

#END
