#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "pwmBuzzer.h"

// Struct for the input num for mapping
typedef struct {
    uint32_t period;		// Intensity of sound (respective to frequency) to be played
    uint32_t dutyCycle;		// Cycle for time (set for 2000)
} pwm_t;

// Pre-mapped tones {period, dutyCycle}
pwm_t intensityTable[] = {
  {200000, 2000}, {150000, 2000}, {100000, 2000}, {50000, 2000} {100, 1}
};

#define BUZZER_INTENSITY_LEVEL_NUM     (sizeof(intensityTable) / sizeof(pwm_t))

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
	TimerLoadSet(WTIMER0_BASE, TIMER_A, 200);
	TimerMatchSet(WTIMER0_BASE, TIMER_A, 200-1);

	// Enable the TimerA
	TimerEnable(WTIMER0_BASE, TIMER_A);
}

void buzzerOn(uint32_t type)
{
	// Setting up tone select
	int typeUse = type - 1;

	// loading Struct mapping 
	pwm_t *pwm = &intensityTable[typeUse];
	
	// Setting sound intensity based on in putted data map
	TimerLoadSet(WTIMER0_BASE, TIMER_A, pwm->period);
	TimerMatchSet(WTIMER0_BASE, TIMER_A, pwm->period - pwm->dutyCycle);
}