#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "pwmBuzzer.h"

void buzzerInit()
{
	// Enable Timer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);

	// Connect pins to those timers
	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinConfigure(GPIO_PC4_WT0CCP0);

	// Selecting PWM for Timer
	TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM));

	// The default setting is very loud, so change it to a quiet version
	TimerLoadSet(WTIMER0_BASE, TIMER_A, 10);
	TimerMatchSet(WTIMER0_BASE, TIMER_A, 10-1);

	// Enable the TimerA
	TimerEnable(WTIMER0_BASE, TIMER_A);
}

void buzzerOn(pwmB_t type)
{
	// Setting sound intensity based on in putted data map
	TimerLoadSet(WTIMER0_BASE, TIMER_A, type.pwmPeriod);
	TimerMatchSet(WTIMER0_BASE, TIMER_A, type.pwmPeriod - type.pwmDutyCycle);
}
