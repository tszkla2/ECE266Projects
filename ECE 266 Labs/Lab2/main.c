/*
 * main.c
 *
 *  Created on: Aug 23, 2016
 *      Author: zzhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "launchpad.h"
#include "seg7.h"

// 7-segment coding table. See https://en.wikipedia.org/wiki/Seven-segment_display. The segments
// are named as A, B, C, D, E, F, G. In this coding table, segments A-G are mapped to bits 0-7.
// Bit 7 is not used in the coding. This display uses active high signal, in which '1' turns ON a
// segment, and '0' turns OFF a segment.
static uint8_t seg7Coding[10] = {
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
};

// The colon status: if colon == 0b10000000, then the colon is on,
// otherwise it is off.

static uint8_t colon = 0;

uint32_t colonhalf = 0;
uint32_t digit1 = 0;
uint32_t digit2 = 0;
uint32_t digit3 = 0;
uint32_t digit4 = 0;

void checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay;

    switch (code) {
    case 1:
        // checking if switch 1 pressed for fast min
        digit3 += 1;
        while(digit3 > 9)       // When the first digit of minutes reaches doubt digits, the digit 1 is set to zero and digit2 is updated respectively
                {
                    digit3 = 0;
                }
        delay = 250;                            // Use an inertia for soft debouncing
        break;

    case 2:
        // checking if switch 2 pressed for fast seconds
        digit1 += 1;
        while(digit1 > 9)       // When the first digit of second reaches doubt digits, the digit 1 is set to zero and digit2 is updated respectively
                {
                    digit1 = 0;
                }
        delay = 250;                            // Use an inertia for soft debouncing
        break;

    default:
        delay = 10;
    }

    schdCallback(checkPushButton, time + delay);
}

// Update the clock display

void clockUpdate(uint32_t time)         // pointer to a 4-byte array
{

    uint8_t code[4];                    // The 7-segment code for the four clock digits

    // Display 00:00 at the begin and updates per 1 seconds
    code[0] = seg7Coding[digit1] + colon;
    code[1] = seg7Coding[digit2] + colon;
    code[2] = seg7Coding[digit3] + colon;
    code[3] = seg7Coding[digit4] + colon;
    seg7Update(code);

    // Changing the colon ':' to on or off respectively on the intervals of 0.5 seonds
    if (colon == 0b00000000)
            {
                colon = 0b10000000;
            }
            else
            {
                colon = 0b00000000;
            }


    if(colonhalf >= 1)      // when 2 half seconds pass (1 second), change time. this first half second changes colon too
    {
        //First Digit
        digit1 = digit1 + 1;

        //Second Digit
        if(digit1 > 9)
        {
            digit2 = digit2 + 1;
        }

        //Third Digit
        if(digit2 > 5)
        {
            digit3 = digit3 + 1;
        }
        //Fourth Digit
        if(digit3 > 9)
        {
            digit4 = digit4 + 1;
        }

        // Time digit update
        while(digit1 > 9)       // When the first digit of second reaches doubt digits, the digit 1 is set to zero and digit2 is updated respectively
        {
            digit1 = 0;
        }
        while(digit2 > 5)       // second digit of seconds cannot go passed 60 thus value updated to 0
        {
            digit2 = 0;
        }
        while(digit3 > 9)       // When the first digit of minutes reaches doubt digits, the digit 1 is set to zero and digit2 is updated respectively
        {
            digit3 = 0;
        }
        while(digit4 > 5)       // second digit of minutes cannot go passed 60 thus value updated to 0
        {
            digit4 = 0;
        }
        while(digit4 == 6)      // Onces 60:00 is reach, an hour has been reach and the counter is reset to 00:00
        {
            digit1 = 0;
            digit2 = 0;
            digit3 = 0;
            digit4 = 0;
        }
        colonhalf--;    // change half second state to 0  to non digit change
    }
    else    // other half second, only change colon
    {
        colonhalf++;    // change half second state to 1 to digit change
    }
    // Call back after 0.5 second
    schdCallback(clockUpdate, time + 500);
}

int main(void)
{
    lpInit();
    seg7Init();

    uprintf("%s\n\r", "Lab 2: Wall clock");

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(clockUpdate, 1000);
    schdCallback(checkPushButton, 50);
    // Loop forever
    while (true) {
        schdExecute();
    }
}
