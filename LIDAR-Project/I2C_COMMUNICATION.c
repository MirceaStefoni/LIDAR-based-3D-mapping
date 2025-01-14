/***********************************************************************************
Name: Octavian 
Author: Diaconescu Octavian
Date: 23-04-19 23:06
Description: I2C functions definitions
************************************************************************************/

#include "I2C_COMMUNICATION.h"

/***********************************************************************************
To add description
************************************************************************************/

void I2C_init()
{
/* Set P0.2 pin as SCL */ 
PINSEL0 |= 1<<4;
PINSEL0 &= ~(1 << 5);
	
/* Set P0.3 pin as SDA */ 
PINSEL0 |= 1<<6;
PINSEL0 &= ~(1 << 7);
	
/* Set bit rate: Bit rate = Pclk / (I2SCLH + I2SCLL)  */ 	
I2CONCLR=0xFF;
I2SCLH=0x1F;
I2SCLL=0xF;	
I2CONSET |=1<<6;
}

/***********************************************************************************
This function is used to initiate a distance measurement.
************************************************************************************/

unsigned int Lidar_start_condition()
{
	unsigned int n = 0;
	unsigned int m = 0;
	I2CONSET |= 1 << 5;
	m = I2STAT;
	while (m != 8)	/* Wait for ack */
	{
		m = I2STAT;
		if (m == 0x00)
		{
			return 1;
		}
	}
	I2CONCLR=0x28;	/* Address shift and write */
	I2DAT = (LIDAR_ADDRESS << 1);		
	n = I2STAT;
	while (n != 0x18) 	/* Wait for ack */
	{
		n = I2STAT;
		if (n == 0x20)
		{
			return 1;
		}
		if (n == 0x00)
		{
			return 1;
		}
	}
	return 0;
} 

/***********************************************************************************
This function is used to stop the distance measurement.
************************************************************************************/

void Lidar_stop_condition()
{
	I2CONSET=0x10;
	I2CONCLR=0x8;
} 

/***********************************************************************************
To add description
************************************************************************************/

uint8_t LidarWrite_8bits(uint8_t address, uint8_t data,uint8_t numBytes)
{
	if (Lidar_start_condition() != 0)
	{
		Lidar_stop_condition();
		return 1;
	}
	
	I2DAT=address;
	I2CONCLR=0x8;
	
	/* Wait for ack */
	if(wait_for_response(0x28)==0)
	{
		I2CONCLR=0x28;
		I2DAT = data;		
		if(wait_for_response(0x28)==0)
		{
			I2CONCLR=0x28;
		}	
		else
		{
			if(DEBUG_MODE == 1)
				printf ("Ack2 in write not received." );
		}
		
	}
	else
	{
		if(DEBUG_MODE == 1)
			printf ("Ack1 in write not received." );
	}	
	Lidar_stop_condition();
	
	return 0x00;
}

/***********************************************************************************
To add description
************************************************************************************/

uint8_t LidarRead_8bits(uint8_t address,uint8_t numBytes)
{
	uint8_t data_read;
	
	if (Lidar_start_condition() != 0)
	{
		Lidar_stop_condition();
		return 1;
	} 
	
	I2DAT=address;
	I2CONCLR=0x8;
	
	/* Wait for ack */
	if(wait_for_response(0x28)==0)
	{
	
		/* Stop */
		I2CONSET=0x10;
		I2CONCLR=0x8;
		
		/* Start */
		I2CONSET |= 1 << 5;
		
		/* Wait for ack */
		if(wait_for_response(0x08)==0)
		{
				/* Read from address*/
				I2CONCLR=0x28;
				I2DAT = (LIDAR_ADDRESS << 1) | 1;		
				
				/* Wait for ack */
			if(wait_for_response(0x40)==0)
			{
				I2CONCLR=0x28;
				
				/* Wait for ack */
				if(wait_for_response(0x58)==0)
				{
				/* Save data */	
					data_read = I2DAT;
				}
				else
				{
				if(DEBUG_MODE == 1)
					printf ("Ack4 in read not received." );
				}
			}
			else
			{
				if(DEBUG_MODE == 1)
					printf ("Ack3 in read not received." );
			}
		}
		else
		{
			if(DEBUG_MODE == 1)
				printf ("Ack2 in read not received." );
		}
	}
	else
	{
		if(DEBUG_MODE == 1)
			printf ("Ack1 in read not received." );
	}
	Lidar_stop_condition();
	
	return data_read;
}

/***********************************************************************************
This function is used to wait for the ack. 
Parameter: code - the expected ack code.
Return : 
Response_code = 0 : akc received.
Response_code = 1 : akc not received.
************************************************************************************/

uint8_t wait_for_response(uint8_t code)
{
	int i=0;
	uint8_t response_code=0x00; 
	while(i<1000000 && I2STAT != code)
	{
		i++;
	}
	if(i>1000000)
		response_code = 0x01;
	
	return response_code;
}
