/***********************************************************************************
Name: Octavian 
Author: Diaconescu Octavian
Date: 23-04-19 23:06
Description: UART functions definitions
************************************************************************************/

#include "UART0.h"

/***********************************************************************************
This function is used to setup the UART comunication on port 0. 
It is always configured on 8 bits, no Parity, 1 stop bit and 9600 baud rate.
************************************************************************************/

void initUART0(void)
{
	PINSEL0 |= (1 << 0) | (1 << 2); // P0.0 P0.1 is COMM 
	IO0DIR |= (1 << 0);  //tx output
	U0LCR |= 3 << 0 | 1 << 7;
	U0DLL = 96; 				 
	//U0DLM = 0;  
	U0LCR = 3;	
	//U0LCR &= 0x0F;
}

/***********************************************************************************
This function is used to write a char on UART port 0. 
Parameters: data - represents the character that need to be transmited.
Return value: 0 if the character was transmitted.
************************************************************************************/

uint8_t U0Write(char data)
{

	
	
	while ( !(U0LSR & THRE ) ); /* Wait till the THR is empty */
	U0THR = data;
	return data;
}

/***********************************************************************************
This function is used to read a char from UART port 0. 
Parameters: 
Return value: a char from the UART 0 port.
************************************************************************************/

char U0Read(void)
{
	unsigned char x;
	
	while( !(U0LSR & RDR ) ); /* Wait till any data arrives into Rx FIFO */
	x = U0RBR;
	return x;
}
