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
#include "rotary.h"
#include "seg7.h"
#include "seg7digit.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"


static uint8_t seg7Coding[11] = {
        0b00111111,         // digit 0
        0b00000110,         // digit 1
        0b01011011,         // digit 2
        0b01001111,         // digit 3
        0b01100110,         // digit 4
        0b01101101,         // digit 5
        0b01111101,         // digit 6
        0b00000111,         // digit 7
        0b01111111,         // digit 8
        0b01101111,         // digit 9
        0b00000000,         // digit BLANK
};

static int rotaryState = 0;

uint32_t digit1 = 0;
uint32_t digit2 = 0;
uint32_t digit3 = 0;
uint32_t digit4 = 0;
uint32_t delay = 50;

void rotaryUse(uint32_t time)
{
    uint8_t code[4];
    uint16_t curNum = rotaryGet();
    double holdNum;

    if(rotaryState == 0)
    {
        holdNum = curNum;
        holdNum = (holdNum / 4090) * 100;
        curNum = (int)holdNum;

        digit1 = curNum % 10;
        curNum = curNum / 10;
        digit2 = curNum % 10;
        curNum = curNum / 10;
        digit3 = curNum % 10;

        digit4 = 10;

        if(digit1 >= 0 && digit2 == 0 && digit3 == 0)
        {
            digit2 = 10;
        }

        if(digit1 >= 0 && digit2 >= 0 && digit3 == 0)
        {
            digit3 = 10;
        }

    }

    else
    {
            digit1 = curNum % 10;
            curNum = curNum / 10;
            digit2 = curNum % 10;
            curNum = curNum / 10;
            digit3 = curNum % 10;
            curNum = curNum / 10;
            digit4 = curNum % 10;


            if(digit1 >= 0 && digit2 == 0 && digit3 == 0 && digit4 == 0)
                    {
                        digit2 = 10;
                    }

            if(digit1 >= 0 && digit2 >= 0 && digit3 == 0 && digit4 == 0)
                    {
                        digit3 = 10;
                    }
            if(digit1 >= 0 && digit2 >= 0 && digit3 >= 0 && digit4 == 0)
                    {
                        digit4 = 10;
                    }
    }

    uprintf("%n", curNum);

    code[0] = seg7Coding[digit1] + 0b00000000;
    code[1] = seg7Coding[digit2] + 0b00000000;
    code[2] = seg7Coding[digit3] + 0b00000000;
    code[3] = seg7Coding[digit4] + 0b00000000;
    seg7Update(code);

    schdCallback(rotaryUse, time + delay);
}

void
checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay;

    switch (code) {
    case 1:
        uprintf("%s\n\r", "1");
        rotaryState = 0;
        delay = 250;
        break;

    case 2:
        uprintf("%s\n\r", "2");
        rotaryState = 1;
        delay = 250;
        break;

    default:
        delay = 50;
        uprintf("%s\n\r", "DEF");
    }

    schdCallback(checkPushButton, time + delay);
}


int
main(void){

    lpInit();
    seg7Init();
    rotaryInit();


	uprintf("%s\n\r", "Lab 6: Knob Control");

	schdCallback(rotaryUse, 1000);
	schdCallback(checkPushButton, 1005);

	while(true)
	{
	  schdExecute();

	}
}
