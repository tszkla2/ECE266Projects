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
	
	// --Configure ADC sequencer #0
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR,0);
	
	// --Configure a single step of the sequencer
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, (ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6));
	
	// --Set the GPIO pin for the motion as input type
	ADCSequenceEnable(ADC0_BASE, 0);
}

uint32_t lightGet()
{
	// ---Variables
	uint32_t readingValue;		// Return value
	
    // ---Getting the Value of light sensor to return
	// --ADC trigger
    ADCProcessorTrigger(ADC0_BASE, 0);
	
    // --Conducting Sampling
	// -Checking for interrupt
    while (!ADCIntStatus(ADC0_BASE, 0, false)) {
    }
	
    // -Getting read value and return it
    ADCSequenceDataGet(ADC0_BASE, 0, &readingValue);
    return readingValue;
}
