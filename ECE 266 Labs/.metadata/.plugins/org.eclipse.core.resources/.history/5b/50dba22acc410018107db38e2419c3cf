/*
 * main.c for Lab 8, spring 2018
 *
 * Created by Zhao Zhang
 */

#include <pwmled.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include "launchpad.h"
#include "pwmBuzzer.h"

static enum {Run, Pause}
sysState = Pause;

/*
 * PWM table for gradual changes.
 */
typedef struct {
    pwm_t pwm;              // PWM parameters
	uint16_t duration;      // Duration of this PWM segment
} pwmSeg_t;

pwmSeg_t pwmTable [] = {
    {{1, 200}, 100},
    {{2, 200}, 100},
    {{3, 200}, 100},
    {{4, 200}, 100},
    {{5, 200}, 100},
    {{6, 200}, 100},
    {{7, 200}, 100},
    {{8, 200}, 100},
    {{9, 200}, 100},
    {{10, 200}, 200},
    {{9, 200}, 100},
    {{8, 200}, 100},
    {{7, 200}, 100},
    {{6, 200}, 100},
    {{5, 200}, 100},
    {{4, 200}, 100},
    {{3, 200}, 100},
    {{2, 200}, 100},
    {{1, 200}, 100}
};

#define	LEGS	(sizeof(pwmTable)/sizeof(pwmTable[0]))

pwmB_t intensityBuzzerTable[] = {
  {191112, 1100},
  {170262, 1100},
  {151686, 1100},
  {143172, 1100},
  {127552, 1100},
  {113636, 1100},
  {101238, 1100},
  {95556, 1100},
  {100, 1}
};

/*
 * Check the push button. SW1 is the RUN key, SW2 is the PAUSE key
 */
void
checkPushButton(uint32_t time)
{
	int code = pbRead();
	uint32_t delay;

	switch (code) {
	case 1:
	    sysState = Run;
		delay = 250;
		break;

	case 2:
	    sysState = Pause;
	    delay = 250;
		break;

	default:
		delay = 10;
	}

	schdCallback(checkPushButton, time + delay);
}

/*
 * Update LED status
 */
void
pwmledUpdate(uint32_t time)
{
    static enum color_t {RED = 0, GREEN = 1, BLUE = 2} color = RED;     // Current color to display
    static int seg = 0;                                                 // Current segment of PWM settings

    if (sysState == Run) {
        // Set up PWM parameters for the three sub-LEDs. Only one LED is lit at a time.
        pwm_t off = {0, 200};
        pwm_t red = (color == RED) ? pwmTable[seg].pwm : off;
        pwm_t green = (color == GREEN) ? pwmTable[seg].pwm : off;
        pwm_t blue = (color == BLUE) ? pwmTable[seg].pwm : off;
        pwmledSetColor(red, green, blue);

        // Move to the next segment, switch color on warp-around
        seg = (seg + 1) % LEGS;
        if (seg == 0) {
            color = (enum color_t) ((color + 1) % 3);
        }

        schdCallback(pwmledUpdate, time + pwmTable[seg].duration);
    }
    else {
        // Check again after 10 ms
        schdCallback(pwmledUpdate, time + 10);
    }
}

int song[]= {2,8,1,8,0,8,1,8,2,8,2,8,2,8,1,8,1,8,1,8,2,8,4,8,4,8,2,8,1,8,0,8,1,8,2,8,2,8,2,8,2,8,1,8,1,8,2,8,1,8,0,8,8,8}; // put song here
int counts = 0;

void soundUpdate(uint32_t time)
{
    pwmB_t mapAr;
	
	if(sysState == Pause){
		mapAr = intensityBuzzerTable[song[counts]];
		buzzerOn(mapAr);
		counts++;
		if(counts == 53)
		{
		    counts = 0;
		}
		schdCallback(soundUpdate, time + 300);
	}
	else
	{
		schdCallback(soundUpdate, time + 200);
	}
	
	//	-Song in words
    // E D C D E E E D D D E G G E D C D E E E E D D E D C
    //    FREQUENCY   CLOCK CYCLES
    // C4 (261.626) = (191112.504)
    // D4 (293.665) = (170262.033)
    // E4 (329.628) = (151686.143)
    // F4 (349.228) = (143172.941)
    // G4 (391.995) = (143172.941)
    // A4 (440.000) = (113636.364)
    // B4 (493.883) = (101238.552)
    // C5 (523.251) = (95556.435)
}

void main(void)
{
	lpInit();
	pwmledInit();
	buzzerInit();

	uprintf("%s\n\r", "Lab 8: Music Player");


	// Schedule the first callback events for LED flashing and push button checking.
	// Those trigger callback chains. The time unit is millisecond.
	schdCallback(checkPushButton, 1005);
	schdCallback(pwmledUpdate, 1010);
	schdCallback(soundUpdate, 1015);

	// Loop forever
	while (true) {
		schdExecute();
	}
}
