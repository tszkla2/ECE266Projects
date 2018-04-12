#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <inttypes.h>
#include "launchpad.h"
#include "ranger.h"

void rangerInit()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP));

    TimerControlEvent(WTIMER0_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);

    TimerEnable(WTIMER0_BASE, TIMER_A);
}

uint32_t rangerGet(){
    uint32_t rise;
    uint32_t fall;
    uint32_t difference;
    uint32_t final;
	
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
    waitUs(2);

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4); 
    waitUs(5);

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);

    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOPinConfigure(GPIO_PC4_WT0CCP0); 

    TimerIntClear(WTIMER0_BASE, TIMER_CAPA_EVENT);

    while(!TimerIntStatus(WTIMER0_BASE,false) ){}
    rise = TimerValueGet(WTIMER0_BASE, TIMER_A);
    TimerIntClear(WTIMER0_BASE, TIMER_CAPA_EVENT);

    while(!TimerIntStatus(WTIMER0_BASE,false) ){}
    fall = TimerValueGet(WTIMER0_BASE, TIMER_A);
    TimerIntClear(WTIMER0_BASE, TIMER_CAPA_EVENT);

    difference = fall - rise;
    uprintf("Working? %" PRIu32 "\n",difference);
    final = (340.0/50000.0)*difference/2.0;
    return final;
}



