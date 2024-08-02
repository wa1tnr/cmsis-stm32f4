#!/bin/sh
# Wed 17 Jul 18:05:01 UTC 2024

erasing() {
	st-flash erase
}

# ${platformio.packages_dir}/tool-stm32duino/stlink/st-flash write $SOURCE 0x8000000

FIRMW=./.pio/build/disco_f407vg/firmware.bin
ADDR=0x08000000

# st-flash --reset write ./.pio/build/disco_f407vg/firmware.bin 0x08000000

# SECOND BEST: uses --reset - fail though (incomplete)
# st-flash --reset write ${FIRMW} ${ADDR}

writing() {
     st-flash --reset write ${FIRMW} ${ADDR}
}

# - - -   m a i n   - - -

erasing
# writing

#END
