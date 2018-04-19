#ifndef PWMBUZZER_H_
#define PWMBUZZER_H_
#include <stdint.h>

// Struct for the input num for mapping
typedef struct {
    uint32_t pwmPeriod;			// Intensity of sound (respective to frequency) to be played
    uint32_t pwmDutyCycle;		// Cycle for time (set for 2000)
} pwmB_t;

/*
 * Initialize the timer and buzzer.
 */
void buzzerInit();

/*
 * play buzzer
 */
void buzzerOn(pwmB_t type);

#endif /* PWMLED_H_ */
