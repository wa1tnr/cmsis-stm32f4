#!/bin/sh
# Wed 17 Jul 19:54:21 UTC 2024


# KNOWN ISSUE
# [ https://github.com/stlink-org/stlink/issues/220 ]
# solved - unstrap BOOT0 and DONE.

erasing() {
	st-flash erase
}

FIRMW=./.pio/build/disco_f407vg/firmware.bin
ADDR=0x08000000

# SECOND BEST: uses --reset - fail though (incomplete)
# st-flash --reset write ${FIRMW} ${ADDR}

writing() {
	st-flash write ${FIRMW} ${ADDR}
}

echoes_floyd() {
	echo;echo;date;echo
}

# - - -   m a i n   - - -

# erasing
echoes_floyd
writing
echoes_floyd

#END
