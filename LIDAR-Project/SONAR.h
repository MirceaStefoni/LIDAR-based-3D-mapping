#ifndef __SONAR_H
#define __SONAR_H
#include <lpc22xx.h>
#include <stdint.h>
#include <stdio.h>
#include "DEBUG_HEADER.h"


void timer_capture_init(void);
	
void toggle(void);

uint16_t read_distance_sonar(void);


#endif