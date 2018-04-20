/*
 * buzzer.h: Basic driver functions for the buzzer.
 * This code is designed for Lab 4 Motion Detection Alarm
 *
 *  Created on: Sep 20, 2016
 *      Author: zzhang
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

// Initialize the Display
void displayInit();

// Display backlighting
void displayOn();

// Display the text
void displayAct(char toDisplay);

#endif /* DISPLAY_H_ */
