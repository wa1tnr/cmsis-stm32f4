/***
 * 
 *   WeAct Black Pill - STM32F411
 * 
 *   has the blue LED on PC13 which uses negative logic
 *   (ground PC13 to light the LED)
 *
 ***/

/***
 * 
 * NEWS - blue LED blinks SLOWLY - 'it works' as the kids say!
 * 
 * RESET and ready-for-next firmware upload is possibly non-trivial
 * to implement correctly (CMSIS hands tied behind back).
 * 
 * It *is* the Arduino toolchain so NVIC_Reset(); type function
 * is likely written already.
 * 
 * Tue 16 Jul 01:25:54 UTC 2024
 * 
 * imported from blue pill work done this week
 * everything may be wrong here.  Still a good model.
 * ideas are okay as is.
 * 
 ***/

#include <stdbool.h>
#include <stdint.h>
#include <stm32f4xx.h>

// want: WeAct black pill target blue onboard LED port and pin
// TODO: see above

// PC13
// same as blue pill PC13 - note it is inverted (ground it to light LED)

#define pinNumber 13
// #define GPIO_OUTPUT_2MHz (0b10)
// #define GPIO_OUTPUT_PUSH_PULL (0 << 2)

// ref. https://stackoverflow.com/questions/59546305/stm32f103-gpio-ports
// RCC_APB2ENR  112, 146    GPIOx_CRH  172

void primary(void) {
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC -> MODER |= GPIO_MODER_MODER13_0; // ((uint32_t)0x0C000000)
    GPIOC -> OTYPER |= GPIO_OTYPER_OT_13; // ((uint32_t)0x00002000)
    GPIOC->OTYPER   = 0;
    GPIOC->OSPEEDR  = 0;

    // const int N = 500000;
    const int N = 5123456;

#if 0
#define GPIO_BSRR_BR_13  ((uint32_t)0x20000000)
#endif

    while (1) {
        // GPIOC -> BSRR = (1 << pinNumber);
        // GPIOC -> ODR ^=   GPIO_ODR_OD13;
        GPIOC -> BSRR = GPIO_BSRR_BR_13;
        for (volatile int i = 0; i < N; i++);
        // GPIOC -> BSRR = (1 << (pinNumber + 16));
        // GPIOC -> ODR ^=   GPIO_ODR_OD13;
        GPIOC -> BSRR = GPIO_BSRR_BS_13; // invert blue led signal
        for (volatile int i = 0; i < N; i++);
    }
}

int main(void) {
    primary();
    return 0;
}

/* end. */
