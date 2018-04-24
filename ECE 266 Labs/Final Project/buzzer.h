/*
 * buzzer.h: Basic driver functions for the buzzer.
 * This code is designed for Lab 4 Motion Detection Alarm
 *
 *  Created on: Sep 20, 2016
 *      Author: zzhang
 */

#ifndef BUZZER_H_
#define BUZZER_H_

typedef struct {
    uint32_t pwmPeriod;         // Intensity of sound (respective to frequency) to be played
    uint32_t pwmDutyCycle;      // Cycle for time (set for 2000)
} pwmB_t;

// Initialize the buzzer
void buzzerInit();

// Turn on the buzzer
void buzzerOn();

// Turn off the buzzer
void buzzerOff();

#endif /* BUZZER_H_ */
