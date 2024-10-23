#include <stdio.h>
#include <stdint.h>
#include <lpc22xx.h>
#include "UART0.h"
#include "TIMER.h"
#include "I2C_COMMUNICATION.h"
#include "Lidar.h"
#include "DEBUG_HEADER.h"
#include "SONAR.h"
#include <math.h>

#define PWM_FREQUENCY 50 // Servo motor PWM frequency in Hz
#define PWM_PERIOD_MS 20 // PWM period in milliseconds (50 Hz => 20 ms)

/***********************************************************************************
Global variables
************************************************************************************/

volatile int i;
uint16_t distance_lidar_hex;
uint16_t distance_sonar_hex;


/***********************************************************************************
This function is executed 1 time at power on or at reset.
************************************************************************************/

void setup_function(void)
{
	initUART0();
	initTimer();
	I2C_init();
	//timer_capture_init();
}

/***********************************************************************************
This function is executed 1 time at power on or at reset.
************************************************************************************/

void blink_led ()
{
		//reg for setting the function of the pin p0.30
		//PINSEL1 &= (1 << 28);
		//PINSEL1 &= (1 << 29);
		//IO0DIR |= 1 << 30; // P0.30 LED
		//Setup for a led to blink
		//IO0SET = 1 << 30;
		//delayMS(1000);
		//IO0CLR = 1 << 30;
		//delayMS(1000);
		//UART echo
		//char c = U0Read();
		//U0Write(c);
		//continue;
}

/***********************************************************************************
Main program
************************************************************************************/


/*
int main(void)
{	

	setup_function();
	Lidar_config(1,0);
	int x=0;
	int y=0;
		
	while(1)
	{
			int medie = 0;
			for(int i=0;i<5;i++)
			{
				medie += read_distance();
			}
		
		if(DEBUG_MODE == 1)
		{
				
					//printf("%d\n",medie/5);
					printf ("%d-%d-%d\n",x,y,medie/5 );
					//        x  y  z
		}
		
		for(int l=0;l<500000;l++);
		
		x=x+5;
		
		if(x==180)
		{
			x=0;
			y=y+5;
		}
		if(y==180)
		{
			y=0;
		}
		
		
	}
	
	
	return 0;
}
*/



void delay_ms(unsigned int ms)
{
    unsigned int i, j;
	
    for (i = 0; i < ms; i++)
        for (j = 0; j < 10000; j++);
}

void servo_init() {
    // Initialize PWM pin as output
	
    PINSEL0 |= (2 << 18); // Select PWM function for the pin p0.9 PWM 6
	
		PINSEL1 |= (1 << 10); // Select PWM function for the pin p0.21 PWM 5
	
		PWMPCR = 0;
	
    PWMPR = 14.7456 - 1; // Set prescaler to 0
    
		PWMMR0 = 20000; // Set PWM period   20ms
	
		PWMMR6 = 500; // Set initial pulse width to minimum (0 degree) PWM 6
	
		PWMMR5 = 2000; // Set initial pulse width to minimum (0 degree) PWM 5
	
		PWMMCR = (1 << 1); // Reset on PWMMR0
	
		PWMLER = (1<<0) | (1<<6); // Enable PWM6 Latch Enable
	
		PWMLER |= (1<<5); 				// Enable PWM5 Latch Enable
  
		PWMPCR |= (1 << 14); // PWM 6
		
		PWMPCR |= (1 << 13); // PWM 5
	
		PWMTCR = (1<<1) ; //Reset PWM TC & PR
    
    PWMTCR = (1 << 0) | (1 << 3); // Enable PWM counter and PWM mode
}

void servo_set_position_x(uint32_t pulse_width) {
    // Convert angle to pulse width
    // Angle range: 0 to 180 degrees
    // Pulse width range: 500 to 2500 microseconds
		
    
    // Set pulse width
    PWMMR6 = pulse_width;
    PWMLER = (1 << 6); // Latch MR6
}

void servo_set_position_y(uint32_t pulse_width) {
    // Convert angle to pulse width
    // Angle range: 0 to 180 degrees
    // Pulse width range: 500 to 2500 microseconds
		
    
    // Set pulse width
    PWMMR5 = pulse_width;
    PWMLER = (1 << 5); // Latch MR5
}


int main(void)
{   
	
	setup_function();
	Lidar_config(1,0);
	
	servo_init();            // SERVO
	
	int poz_y = 2000;
	
	double x = 30;
	double y = 110;
	int z = 0;
	
	delay_ms(7000);
	
	while(1)
	{
		
		//toggle();
		
		//distance_sonar_hex = read_distance_sonar();
		
			
			/*
			if(distance_sonar_hex <= 400)
			{
				printf("Distance Sonar: %d cm\n",distance_sonar_hex);
			}
			*/
			
				x = 30;
				for(int i=500;i<=1900;i=i+25)
				{
					servo_set_position_x(i);
					delay_ms(75);
					
					int suma = 0;
					for(int k=0;k<5;k++)
					{
						suma += read_distance();
						delay_ms(1);
					}
					z = suma/5;
					printf ("%g-%g-%d\n",x,y,z);
					x += 3.928/2;	
				}
				
				poz_y -= 20;
				servo_set_position_y(poz_y);   //1800 poz initiala
				delay_ms(100);
				y -= 1.9;
				
				
				x=140;
				for(int i=1900;i>=500;i=i-25)
				{
					servo_set_position_x(i);
					delay_ms(75);
					
					int suma = 0;
					for(int k=0;k<5;k++)
					{
						suma += read_distance();
						delay_ms(1);
					}
					z = suma/5;
					
					printf ("%g-%g-%d\n",x,y,z);
					
					x -= 3.928/2; 
				}
				
				
				poz_y -= 20;
				servo_set_position_y(poz_y);   //1800 poz initiala
				delay_ms(100);
				y -= 1.9;

				if(poz_y <= 1600)
				{
						servo_set_position_y(1800);   //1800 poz initiala
						delay_ms(250);
						servo_set_position_x(1200);   //1200 poz mijloc
					
					return 0;
				}
				
				
			
		
		//for (i = 0; i < 300000; i++);
		
		//for(int i=0; i<3000000;i++); //600000 == 80 us
	}
	
	return 0;
}


