#ifndef RANGER_H_
#define RANGER_H_

#include <stdint.h>
// Initialize the motion detector
void rangerInit();

//Function for detecting
uint16_t rangerGet();

static void sendStartPulse();

#endif // RANGER_H_INCLUDED
