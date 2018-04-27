#include <stdint.h>
#include <stdbool.h>
#include <launchpad.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <driverlib/adc.h>
#include "light.h"

void lightInit()
{
	// ---Initialize the GPIO peripheral for the port that the light sensor uses to function
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	
	// --Waiting on device
	while(SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0) != 1){
		// Empty loop
	}
	
	// --Configure ADC sequencer #0
	ADCSequenceConfigure(GPIO_PIN_3, 0, ADC_TRIGGER_PROCESSOR,0);
	
	// --Configure a single step of the sequencer
	ADCSequenceStepConfigure(GPIO_PIN_3, 0, 0, (ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2));
	
	// --Set the GPIO pin for the motion as input type
	ADCSequenceEnable(GPIO_PIN_3, 0);
	
	// --End of function
	return 0;
}

uint32_t lightGet()
{
	// ---Variables
	int i = 0;			// Checking for loop interrupt by pass of read value
	
	// ---Getting the Value of light sensor to return
	// --Wait for ADC to trigger
	ADCProcessorTrigger(GPIO_PIN_3, 0);
	
	// --Conducting Sampling
	// -Checking for interrupt
	while(i == 0){
		i = ADCIntStatus(GPIO_PIN_3, 0, 0);
	}
	
	// -Getting read value and return it (done ASM as c version doesn't work)
	__asm__("LDR   r0, ROTARY_PORT\n\t"
			"MOV   r1, #0\n\t"
			"SUB   SP, #4\n\t"
			"MOV   r2, sp\n\t"
			"BL    ADCSequenceDataGet\n\t"
			"POP {r0, pc}\n\t");  //pop adcReading to r0 and return
}
