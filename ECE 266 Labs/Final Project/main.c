/*
 * main.c for Final Project, spring 2018
 *
 */

#include <pwmled.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include "launchpad.h"
#include "pwmBuzzer.h"
#include "light.h"

// --- System status variables
uint8_t sysStateLed = 0;	// LED Status
uint8_t sysStateBuz = 0;	// Activates buzzer when 1
uint8_t sysTimeCur = 0;		// States if it day time (1) or night time (0)
uint8_t sysNightTra = 0;	// Current music track for night time
uint8_t sysDayTra = 0;		// Current music track for day time
int counts = 0;			// Reference for song location in song

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

pwmB_t intenBuzzerTabDay[] = {
  {191112, 2000}, // DIF 190012
  {170262, 2000}, // DIF 169162
  {151686, 2000}, // DIF 150586
  {143172, 2000}, // DIF 142072
  {127552, 2000}, // DIF 126452
  {113636, 2000}, // DIF 112536
  {101238, 2000}, // DIF 100138
  {95556, 2000},
  {100, 1}
};

pwmB_t intenBuzzerTabNight[] = {
  {191112, 1000}, // DIF 190012
  {170262, 1000}, // DIF 169162
  {151686, 1000}, // DIF 150586
  {143172, 1000}, // DIF 142072
  {127552, 1000}, // DIF 126452
  {113636, 1000}, // DIF 112536
  {101238, 1000}, // DIF 100138
  {95556, 1000},
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
	case 1:		// On and off music
	    if(sysStateBuz == 0)
	        sysStateBuz = 1;
	    else if(sysStateBuz == 1)
	        sysStateBuz = 0;
	    delay = 250;
		break;

	case 2:		// Change Track
	    if(sysTimeCur == 1){ 	// Day time
			sysDayTra++;
			if(sysDayTra > 2)
				sysDayTra = 0;
			count = 0;
		}
		else{		// Night time
			sysNightTra++;
			if(sysNightTra > 1)
				sysNightTra = 0;
			count = 0;
		}
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

    if (sysStateLed == 1) {
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

// ---Tracks Definitions:
int daySongs[3][]		// Call for day songs 
int daySongs[0][]= {};	// Day Track 1: 
int daySongs[1][]= {};	// Day Track 2: 
int daySongs[2][]= {};	// Day Track 3: 
int nightSongs[2][];	// Call for night songs
int nightSongs[0][]= {2,8,1,8,0,8,1,8,2,8,2,8,2,8,1,8,1,8,1,8,2,8,4,8,4,8,2,8,1,8,0,8,1,8,2,8,2,8,2,8,2,8,1,8,1,8,2,8,1,8,0,8,8,8};	// Night Track 1: Mary had a little lamb
int nightSongs[1][]= {};	// Night Track 2:

void soundUpdate(uint32_t time)
{
	// Setting up for playing songs
	if(sysStateBuz == 1){
		if(lightGet >= )	// If day time		!!! need DATA values
			buzzerOn(intenBuzzerTabDay[daySongs[sysDayTra][count]].pwmPeriod, intenBuzzerTabDay[daySongs[sysDayTra][count]].pwmPeriod - intenBuzzerTabDay[daySongs[sysDayTra][count]].pwmDutyCycle);
			counts++;
			if(counts < strlen(daySongs[sysDayTra][]))		// !!! Check count
			{
				counts = 0;
			}
			schdCallback(soundUpdate, time + 300);
		else{		// If night time
			buzzerOn(intenBuzzerTabNight[nightSongs[sysNightTra][count]].pwmPeriod, intenBuzzerTabNight[nightSongs[sysNightTra][count]].pwmPeriod - intenBuzzerTabNight[nightSongs[sysNightTra][count]].pwmDutyCycle);
			counts++;
			if(counts < strlen(nightSongs[sysNightTra][]))		// !!! Check count
			{
				counts = 0;
			}
			schdCallback(soundUpdate, time + 300);
		}
	}
	else
	{
		schdCallback(soundUpdate, time + 200);
	}
	
	//	--Song conversion table
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
	lightInit();

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
