/***********************************************************************************
Name: Octavian 
Author: Diaconescu Octavian
Date: 23-04-19 23:06
Description: Timers header file
************************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H
#include <lpc22xx.h>
#include "DEBUG_HEADER.h"
#include <stdio.h>
#include <stdint.h>

#define PRESCALE 14000 

/***********************************************************************************
To add description 
************************************************************************************/

void initTimer(void);
void delayMS(unsigned int milliseconds);

#endif
