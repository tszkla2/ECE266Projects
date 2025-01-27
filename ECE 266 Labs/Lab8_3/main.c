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
#include "light.h"

//static enum {Run, Pause}
//sysState = Pause;
uint8_t sysStateLed;
uint8_t sysStateBuz;
uint8_t currSong = 0;
int counts = 0;

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

  {227272, 15000}, // A3 - 0
  {202478, 15000}, // B3 - 1
  {191112, 15000}, // C4 - 2
  {170264, 15000}, // D4 - 3
  {151685, 15000}, // E4 - 4
  {143172, 15000}, // F4 - 5
  {127552, 15000}, // G4 - 6
  {113636, 15000}, // A4 - 7
  {101238, 15000}, // B4 - 8
  {95556, 15000},  // C5 - 9
  {85131, 15000},  // D5 - 10
  {75843, 15000},  // E5 - 11
  {71586, 15000},  // F5 - 12
  {63776, 15000},  // G5 - 13
  {100, 1}        // Blank - 14
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
	    if(sysStateLed == 0)
	        sysStateLed = 1;
	    else if(sysStateLed == 1)
	        sysStateLed = 0;
	    currSong++;
		counts = 0;
	    if(currSong == 2)
	        currSong = 0;
	    buzzerOn(intensityBuzzerTable[14].pwmPeriod, intensityBuzzerTable[14].pwmPeriod - intensityBuzzerTable[14].pwmDutyCycle);
		delay = 250;
		break;

	case 2:
	    if(sysStateBuz == 0)
	        sysStateBuz = 1;
	    else if(sysStateBuz == 1){
	        sysStateBuz = 0;
            buzzerOn(intensityBuzzerTable[14].pwmPeriod, intensityBuzzerTable[14].pwmPeriod - intensityBuzzerTable[14].pwmDutyCycle);
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
    static enum color_t {RED = 0, GREEN = 1, BLUE = 2} color = BLUE;     // Current off state
    static int seg = 4;                                                 // Current segment of PWM settings
    int lightSens = lightGet();                                         // Current Light sensor state

    if (sysStateLed == 1) {
        // Set up PWM parameters for the three sub-LEDs. Only one LED is lit at a time.
        pwm_t off = {0, 200};
        if(lightSens >= 2000){   // Day time
            color = RED;
        }
        else{
            color = BLUE;
        }
        pwm_t red = (color == RED) ? pwmTable[seg].pwm : off;
        pwm_t green = off;
        pwm_t blue = (color == BLUE) ? pwmTable[seg].pwm : off;
        pwmledSetColor(red, green, blue);

        schdCallback(pwmledUpdate, time + pwmTable[seg].duration);
    }
    else {
        // Check again after 10 ms
        schdCallback(pwmledUpdate, time + 10);
    }
}

// ---LIST OF SONGS:
int song[]= {4,14,3,14,2,14,3,14,4,14,4,14,4,14,3,14,3,14,3,14,4,14,6,14,6,14,4,14,3,14,2,14,3,14,4,14,4,14,4,14,4,14,3,14,3,14,4,14,3,14,2,14,14,14}; //Mary Had 54
//int song2[]= {0,14,3,14,3,14,3,14,4,14,5,14,5,14,5,14,4,14,3,14,4,14,5,14,3,14,5,14,5,14,6,14,7,14,7,14,6,14,5,14,6,14,7,14,5,14,3,14,3,14,4,14,5,14,5,14,4,14,3,14,4,14,5,14,3,14,0,14,0,14,3,14,3,14,3,14,4,14,5,14,5,14,5,14,4,14,3,14,4,14,5,14,3,14,14,14};// Spider
int song3[]= {3,14,6,14,6,14,3,14,3,14,4,14,4,14,3,14,3,14,6,14,6,14,7,14,7,14,8,14,6,14,8,14,8,14,9,14,9,14,9,14,7,14,7,14,8,14,8,14,8,14,6,14,6,14,7,14,7,14,7,14,6,14,5,14,3,14,4,14,5,14,6,14,6,14,14,14}; // Bingo 76
//int song4[]= {3,14,3,14,5,14,5,14,4,14,5,14,4,14,3,14,5,14,5,14,7,14,7,14,6,14,7,14,6,14,5,14,8,14,8,14,8,14,7,14,7,14,7,14,8,14,8,14,8,14,7,14,6,14,6,14,6,14,5,14,5,14,5,14,4,14,5,14,4,14,3,14,14,14}; // Humpty
//int song5[]= {1,14,3,14,8,14,7,14,6,14,1,14,3,14,6,14,5,14,2,14,3,14,9,14,6,14,7,14,7,14,6,14,4,14,3,14,1,14,3,14,8,14,7,14,6,14,1,14,3,14,6,14,5,14,2,14,6,14,9,14,8,14,6,14,7,14,4,14,5,14,6,14,14,14}; //Rockabye
int song6[]={2,14,2,14,6,14,6,14,7,14,7,14,6,14,5,14,5,14,4,14,4,14,3,14,3,14,2,14,6,14,6,14,5,14,5,14,4,14,4,14,3,14,6,14,6,14,5,14,5,14,4,14,4,14,3,14,2,14,2,14,6,14,6,14,7,14,7,14,6,14,5,14,5,14,4,14,4,14,3,14,3,14,2,14,14,14}; //Twinkle 86
int song7[]={4,14,4,14,4,14,4,14,4,14,4,14,4,14,6,14,2,14,3,14,4,14,5,14,5,14,5,14,5,14,5,14,4,14,4,14,4,14,4,14,3,14,3,14,4,14,3,14,6,14,4,14,4,14,4,14,4,14,4,14,4,14,4,14,6,14,2,14,3,14,4,14,5,14,5,14,5,14,5,14,5,14,4,14,4,14,4,14,6,14,6,14,5,14,3,14,2,14,14,14}; //Jingle 98

void soundUpdate(uint32_t time)
{
	int lightSens = lightGet();
	if(sysStateBuz == 1){
	 if(lightSens >= 2000){   // Day time
	    if(currSong == 0){
	        buzzerOn(intensityBuzzerTable[song3[counts]].pwmPeriod, intensityBuzzerTable[song3[counts]].pwmPeriod - intensityBuzzerTable[song3[counts]].pwmDutyCycle);
	        counts++;
	        if(counts == 75)
	        {
	            counts = 0;
	        }
	        schdCallback(soundUpdate, time + 300);
	    }
	    else if(currSong == 1){
	        buzzerOn(intensityBuzzerTable[song7[counts]].pwmPeriod, intensityBuzzerTable[song7[counts]].pwmPeriod - intensityBuzzerTable[song7[counts]].pwmDutyCycle);
	        counts++;
	        if(counts == 98)
	        {
	            counts = 0;
	        }
	        schdCallback(soundUpdate, time + 300);
	    }
	    else
	    {
	        schdCallback(soundUpdate, time + 200);
	    }
	  }
	  else{   // Night time
	    if(currSong == 0){
	        buzzerOn(intensityBuzzerTable[song[counts]].pwmPeriod, intensityBuzzerTable[song[counts]].pwmPeriod - intensityBuzzerTable[song[counts]].pwmDutyCycle);
	        counts++;
	        if(counts == 53)
	        {
	            counts = 0;
	        }
	        schdCallback(soundUpdate, time + 300);
	    }
	    else if(currSong == 1){
	        buzzerOn(intensityBuzzerTable[song6[counts]].pwmPeriod, intensityBuzzerTable[song6[counts]].pwmPeriod - intensityBuzzerTable[song6[counts]].pwmDutyCycle);
	        counts++;
	        if(counts == 85)
	        {
	            counts = 0;
	        }
	        schdCallback(soundUpdate, time + 300);
	    }
	    else
	    {
            schdCallback(soundUpdate, time + 200);
	    }
	  }
	}
	else
	{
		schdCallback(soundUpdate, time + 200);
	}
	
	//	-Song in words
    // E D C D E E E D D D E G G E D C D E E E E D D E D C
    //    FREQUENCY   CLOCK CYCLES
	// A3 (220.000) = (227272.727)
	// B3 (246.94) = (202478.335)
	// C4 (261.626) = (191112.504)
    // D4 (293.665) = (170262.033)
    // E4 (329.628) = (151686.143)
    // F4 (349.228) = (143172.941)
    // G4 (391.995) = (143172.941)
    // A4 (440.000) = (113636.364)
    // B4 (493.883) = (101238.552)
    // C5 (523.251) = (95556.435)
	// D5 (587.33) = (85131.017)
	// E5 (659.25) = (75843.762)
	// F5 (698.46) = (71586.061)
	// G5 (783.99) = (63776.324)
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
