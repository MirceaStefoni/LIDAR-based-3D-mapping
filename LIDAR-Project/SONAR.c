#include "SONAR.h"

volatile double Value = 0;
volatile double Diferenta = 0;
volatile double Total = 0;
volatile double Variabila = 0;
volatile int Flag = 0;

void fiqint(void) __irq
{
	Diferenta = T1CR0 - Value;
	
	Value = T1CR0;
	
	Flag = 1;
	
	T1IR |= 1 << 0;
	
	VICVectAddr = 0x00000000;

}		
	

void timer_capture_init(void)
{
	IO0DIR |= 1 << 23; //set p0.23 as output
	
	PINSEL0 |= 2 << 20; // Set pin p0.10 to capture 1
	
	T1TCR |= 1 << 1; // Reset timer counter
	
	T1CCR |= 7 << 0; // Capture on rising edge
	
	T1TCR = 1 << 0; // Start timer
	
	VICVectCntl1 = 5 | (1 << 5);
	VICVectAddr1 = (unsigned)fiqint;
	
	
	VICIntEnable |= 1 << 5;

}
	

void toggle(void)
{
	
	IO0SET = 1 << 23;   // trimite valoare de 1
	
	for(int i=0; i < 250; i++);   // 20us
	
	IO0CLR = 1 << 23;    // reseteaza valoare
	
}

uint16_t read_distance_sonar(void)
{
	
		if(Flag == 1)
		{
			
			Variabila = 14745600/Diferenta;
			
			Total = (((1/Variabila) * 340)/2)*100;
			
			Flag = 0;
			
			return Total;
		}
	
	return 0;
}
