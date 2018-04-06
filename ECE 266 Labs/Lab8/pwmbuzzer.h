/*
 * pwdled.h: for Lab 8, spring 2018
 *
 *  Created by Zhao Zhang
 */

#ifndef PWMLED_H_
#define PWMLED_H_

#include <stdint.h>

// The PWM parameters
typedef struct {
    uint16_t pulse_width;       // PWM pulse width
    uint16_t period;            // PWM period in cycles
} pwm_t;

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void pwmledInit();

/*
 * Set color of the on-board LED by setting the intensity of the primary colors
 */
void pwmledSetColor(pwm_t red, pwm_t green, pwm_t blue);

#endif /* PWMLED_H_ */
