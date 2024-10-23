/***********************************************************************************
Name: Octavian 
Author: Diaconescu Octavian
Date: 23-04-19 23:06
Description: UART header file
************************************************************************************/

#ifndef __UART0_H
#define __UART0_H
#include <lpc22xx.h>
#include <stdint.h>
#include <stdio.h>
#include "DEBUG_HEADER.h"

#define THRE (1<<5) 
#define RDR (1<<0)

/***********************************************************************************
Those functions were implemented in order to provide a simple UART communication.
************************************************************************************/

void initUART0(void);
uint8_t U0Write(char data);
char U0Read(void);

#endif
