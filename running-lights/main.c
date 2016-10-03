/*
 * Switches LEDs D1-D4 on a button press
 * One at a time, circularily
 */

#include <stdbool.h> /* driverlib requires stdbool.h to be included first */
#include <stdint.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h> /* supplies GPIO_PIN_x */
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h> /* supplies GPIO_PORTx_BASE */

/* controller is clocked with 16 MHz */
#define F_CPU 16000000

void setup();
void light(char leds);

void main(void) {

    /* D1 is on by default */
    char leds = 0b0001;

    setup();

    while (1) {
        light(leds);

        if (!(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) & GPIO_PIN_0)) {
            /* button pressed */
            /* circular shift */
            leds = (leds << 1) | (leds >> 3);
        }

        SysCtlDelay(F_CPU / 30); /* 0.1s */
    }
}

void setup(void) {
    uint32_t ui32Strength;
    uint32_t ui32PinType;

    /* USR_SW1 button */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    /* D1 and D2 leds */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    /* D3 and D4 leds */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* input */
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);

    /* config */
    GPIOPadConfigGet(GPIO_PORTJ_BASE, GPIO_PIN_0, &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, ui32Strength, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigGet(GPIO_PORTN_BASE, (GPIO_PIN_1 | GPIO_PIN_0), &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTN_BASE, (GPIO_PIN_1 | GPIO_PIN_0), ui32Strength, GPIO_PIN_TYPE_STD);
    GPIOPadConfigGet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), ui32Strength,
    GPIO_PIN_TYPE_STD);

    /* output */
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, (GPIO_PIN_1 | GPIO_PIN_0));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0));
}

void light(char leds) {
    /*
     * leds = 0001       ... lights up D1
     * leds = 0100       ... lights up D3
     *
     * (leds >> 2) & 1   ... reads the third bit from right
     * << 4              ... shifts the result to the required pin bit
     */

    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, ((leds >> 0) & 1) << 1);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, ((leds >> 1) & 1) << 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, ((leds >> 2) & 1) << 4);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, ((leds >> 3) & 1) << 0);
}
