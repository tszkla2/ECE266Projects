/*
 * pwmled.c: for Lab 8, spring 2018
 *
 *  Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "pwmled.h"

#define	TIMER0			TIMER0_BASE
#define	TIMER1			TIMER1_BASE
#define PORTF			GPIO_PORTF_BASE

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 *    Red: 		PF1, T0CCP1, Timer 0-B
 *    Blue:		PF2, T1CCP0, Timer 1-A
 *    Green:	PF3, T1CCP1, Timer 1-B
 */
void
pwmledInit()
{
	// Enable Timer 0 and Timer 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	// Connect pins to those timers
	GPIOPinTypeTimer(PORTF, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinConfigure(GPIO_PF1_T0CCP1);
	GPIOPinConfigure(GPIO_PF2_T1CCP0);
	GPIOPinConfigure(GPIO_PF3_T1CCP1);

	// Select PWM for Timer 0 sub-Timer B, and Timer 1 sub-Timers A & B
	TimerConfigure(TIMER0, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));
	TimerConfigure(TIMER1, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM));

	// Invert the PWM waveform, so that the Match register value is the pulse width.
	// Otherwise, the pulse width will be (Load value) - (Match value).
	// The inversion is done by enabling output inversion on the PWM pins.
	TimerControlLevel(TIMER0, TIMER_B, true /* output inversion */);
    TimerControlLevel(TIMER1, TIMER_BOTH, true /* output inversion */);

	// Turn off the LEDs initially
	TimerLoadSet(TIMER0, TIMER_B, 200);
	TimerMatchSet(TIMER0, TIMER_B, 0);
	TimerLoadSet(TIMER1, TIMER_A, 200);
	TimerMatchSet(TIMER1, TIMER_A, 0);
	TimerLoadSet(TIMER1, TIMER_B, 200);
	TimerMatchSet(TIMER1, TIMER_B, 0);

	// Enable the Timer 0's TimerB and Timer 1's TimerA and TimerB
	TimerEnable(TIMER0, TIMER_B);
	TimerEnable(TIMER1, TIMER_A | TIMER_B);
}

/*
 * Set color of the on-board LED by setting the intensity of the primary colors
 */
void
pwmledSetColor(pwm_t red,
			   pwm_t blue,
			   pwm_t green)
{
    // Set the period and duty cycle length for the read sub-LED
	TimerLoadSet(TIMER0, TIMER_B, red.period);
	TimerMatchSet(TIMER0, TIMER_B, red.pulse_width);

    // Set the period and duty cycle length for the read sub-LED
	TimerLoadSet(TIMER1, TIMER_A, blue.period);
	TimerMatchSet(TIMER1, TIMER_A, blue.pulse_width);

    // Set the period and duty cycle length for the read sub-LED
	TimerLoadSet(TIMER1, TIMER_B, green.period);
	TimerMatchSet(TIMER1, TIMER_B, green.pulse_width);
}
