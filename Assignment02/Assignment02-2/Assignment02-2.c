#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <time.h>

void delay(int tm);
void intia_port_F();
void blinkLED(int tm);
void intia_port_C();
void blinkLED_n(int n);

int main(){
	int blink_times = 3;
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
	
	//Pin directions for Port C, set pin 4,5,6,7 to be output
	GPIO_PORTC_DIR_R |= 0xF0;
	
	//Enable digital mode operation GPIO_DEN, enable pin 4,5,6,7 digital mode operation
	GPIO_PORTC_DEN_R |= 0xF0;
}

void blinkLED(int tm){
	//time_t start;
	
	while(1)
	{		
		//start = time(NULL);
		// Data register
		//red light on - pin 1 activate
		GPIO_PORTF_DATA_R = 0x02;
		//while (time(NULL) < start + tm);
		delay(tm);
		
		//start = time(NULL);
		//blue light on - pin 2 activate
		GPIO_PORTF_DATA_R <<= 1;
		//while (time(NULL) < start + tm);
		delay(tm);
	
		//start = time(NULL);
		//green light on - pin 3 activate
		GPIO_PORTF_DATA_R <<= 1;
		//while (time(NULL) < start + tm);
		delay(tm);

	}
}

void blinkLED_n(int n){
	// blink green light n times
	for(int i=0; i<n; i++){
		GPIO_PORTF_DATA_R =0x08;
		delay(2000000);
		GPIO_PORTF_DATA_R =0x00;
		delay(2000000);
	}
	// blink blue light n times
	for(int i=0; i<n; i++){
		GPIO_PORTF_DATA_R =0x04;
		delay(2000000);
		GPIO_PORTF_DATA_R =0x00;
		delay(2000000);
	}
	// blink red light n times
	for(int i=0; i<n; i++){
		GPIO_PORTF_DATA_R =0x02;
		delay(2000000);
		GPIO_PORTF_DATA_R =0x00;
		delay(2000000);
	}
}

