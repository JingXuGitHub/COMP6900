#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <time.h>

void delay(int tm);
void intia_port_F();
void blinkLED(int tm);
void intia_port_C();
void blinkLED_n(int n);

int main(){
	int blink_times = 4;
	intia_port_F();
	//blinkLED(2000000);
	blinkLED_n(blink_times);
	return 0;
}

void delay (int tm){
  int cnt =0;
  while (cnt<tm)
		  ++cnt;
}

void intia_port_F (){
	// System clock enable for Port F
	SYSCTL_RCGCGPIO_R |= 0x20;
	
	//Pin directions for Port F
	GPIO_PORTF_DIR_R |= 0x0E;
	
	//Enable digital mode operation GPIO_DEN
	GPIO_PORTF_DEN_R |= 0x0E;
	
}

void intia_port_C(){
	// System clock enable for Port C
	SYSCTL_RCGCGPIO_R |= 0x04;
	
	// Unlock pin 0,1,2,3 of port C
	GPIO_PORTC_LOCK_R = 0x4C4F434B;
	GPIO_PORTC_CR_R |= 0xFF;
	
	//Pin directions for Port C, set pin 1,2,3,4,5,6,7 to be output
	GPIO_PORTC_DIR_R |= 0xFF;
	
	//Enable digital mode operation GPIO_DEN, enable pin 1,2,3,4,5,6,7 digital mode operation
	GPIO_PORTC_DEN_R |= 0xFF;
}

void blinkLED(int tm){

	while(1)
	{
		// Data register
		//red light on - pin 1 activate
		GPIO_PORTF_DATA_R = 0x02;
		delay(tm);
		
		//blue light on - pin 2 activate
		GPIO_PORTF_DATA_R <<= 1;
		delay(tm);
	
		//green light on - pin 3 activate
		GPIO_PORTF_DATA_R <<= 1;
		delay(tm);

	}
}

void blinkLED_n(int n){
	int c=0;
	while(1){
		if(c<n){
			GPIO_PORTF_DATA_R = 0x08;
			delay(3000000);
			GPIO_PORTF_DATA_R = 0x00;
			delay(3000000);
			c++;
		}
		else if(c<2*n){
			GPIO_PORTF_DATA_R = 0x04;
			delay(3000000);
			GPIO_PORTF_DATA_R = 0x00;
			delay(3000000);
			c++;
		}
		else if(c<3*n){
			GPIO_PORTF_DATA_R = 0x02;
			delay(3000000);
			GPIO_PORTF_DATA_R = 0x00;
			delay(3000000);
			c++;
		}
		else{
			c=0;
		}
	}
}

