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

#define F_CPU 16000000 /* controller is clocked with 16 MHz */
#define LED_COUNT 4
#define OPEN 1
#define CLOSED 0

void setup();
void light(char leds);
char bit_read(char byte, char bit);
void bit_write(char *byte, char bit, char data);

void main(void) {

    char leds = 0b0001; /* D1 is on by default */
    char button;

    setup();

    while (1) {
        light(leds);

        button = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0);

        if (bit_read(button, 0) == CLOSED) {
            /* circular shift */
            leds = (leds << 1) | (leds >> LED_COUNT-1);
        }

        SysCtlDelay(F_CPU / 30); /* 0.1s */
    }
}

void setup(void) {
    uint32_t ui32Strength;
    uint32_t ui32PinType;

    /* USR_SW1 and USR_SW2 buttons */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    /* D1 and D2 leds */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    /* D3 and D4 leds */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* input */
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, (GPIO_PIN_0 | GPIO_PIN_1));

    /* config */
    GPIOPadConfigGet(GPIO_PORTJ_BASE, (GPIO_PIN_0 | GPIO_PIN_1), &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, (GPIO_PIN_0 | GPIO_PIN_1), ui32Strength, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigGet(GPIO_PORTN_BASE, (GPIO_PIN_1 | GPIO_PIN_0), &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTN_BASE, (GPIO_PIN_1 | GPIO_PIN_0), ui32Strength, GPIO_PIN_TYPE_STD);
    GPIOPadConfigGet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), ui32Strength, GPIO_PIN_TYPE_STD);

    /* output */
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, (GPIO_PIN_1 | GPIO_PIN_0));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0));
}

void light(char leds) {
    /*
     * leds = 0001       ... lights up D1
     * leds = 0100       ... lights up D3
     */

    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, bit_read(leds, 0) << 1);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, bit_read(leds, 1) << 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, bit_read(leds, 2) << 4);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, bit_read(leds, 3) << 0);
}

char bit_read(char byte, char bit) {
    /* reads the nth bit from right */
    return (byte >> bit) & 1;
}

void bit_write(char *byte, char bit, char data) {
    /* writes the nth bit from right */
    *byte = *byte & ~(1 << bit) | (data << bit);
}
