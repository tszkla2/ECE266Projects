 /*
 * main.c: For Lab 5, Sleeping LaunchPad
 *
 * Created by Zhao Zhang.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include "launchpad.h"
#include "motion.h"

// Color defines
#define     RED         0
#define     BLUE        1
#define     GREEN       2

// Color display setting
volatile int colorSetting = BLUE;

static bool colorTable[3][3] = {
    {true, false, false},       // Red
    {false, true, false},       // Blue
    {false, false, true}        // Green
};

/*
 * LED flashing function. Note that this function must run in a thread, e.g. not be callback-based,
 * because the frequency of buzzer on/off is too high for the callback scheduling module.
 */
void
ledFlash()
{
    int i;

    while (true) {
            IntMasterDisable();     // disable all interrupts so that the color stays the same

            for (i = 0; i < 3; i++) {
                ledTurnOnOff(colorTable[colorSetting][0], colorTable[colorSetting][1],
                             colorTable[colorSetting][2]);
            waitMs(300);
            ledTurnOnOff(false, false, false);
            waitMs(300);
        }

            IntMasterEnable();      // enable interrupt signal from interrupt controller to CPU

            // The following is in-line assembly, i.e. inserting assembly instructions into C code
            // Execute an WFI instruction to put the processor into the sleep mode
            __asm("    wfi");
    }
}

/*
 * Interrupt handler for both push buttons (pins PF0 and PF4)
 */
void
pbIntrHandlerSW()
{
    // Clear interrupt. This is necessary, otherwise the interrupt handler will be executed forever.
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_5);


        int code = pbRead();
        switch (code) {
        case 1: // Setting light to green if SW1 pressed
            colorSetting = GREEN;
            break;

        case 2: // Setting light to blue if SW2 pressed
            colorSetting = BLUE;
            break;

        default: // The only other activation of the light is from the motion sensor thus the light is red
            colorSetting = RED;
        }
}

/*
 * Select a set of interrupts that can wake up the LaunchPad
 */
void
setInterrupts()
{
	// Disable the system tick interrupt, which has been enabled in lpInit();
	// otherwise, it will wake up Tiva C.
	SysTickIntDisable();

	// Set interrupt on Port F, pin 0 (SW1) and pin 4 (SW2)        
	GPIOIntRegister(GPIO_PORTF_BASE, pbIntrHandlerSW);
	// Set interrupt on Port C, pin 5 (motion)        
	GPIOIntRegister(GPIO_PORTC_BASE, pbIntrHandlerSW);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
	GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0,     // interrupt on falling edge, note that SW1 and SW2 are active low
	            GPIO_FALLING_EDGE);
	GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_5,     // interrupt on falling edge, note that SW1 and SW2 are active low
	                GPIO_FALLING_EDGE);
	IntPrioritySet(INT_GPIOF, 0);
	IntPrioritySet(INT_GPIOC, 0);
}

int
main(void){
	// Initialize the launchpad, buzzer
	lpInit();

	// IN YOUR SOLUTION CODE, call the function that initializes the PIR montion sensor,
	//    which you wrote in Lab 4, e.g.

	motionInit();
	// NOTE: In this lab, you should NOT use the callback scheduler because it will wake up the CPU
	// from sleeping.

	setInterrupts();

	uprintf("%s\n\r", "Lab 5: Sleeping LaunchPad");

	ledFlash();
}
