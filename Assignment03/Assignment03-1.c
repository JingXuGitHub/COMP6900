#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <time.h>

void delay(int tm);
void PortAF_Init(void);
void SwitchInput(void);
void normalMode(void);
void emergencyMode(void);
void selectMode(void);
void closeSystem(void);

int state_big = 0;
int state_small = 0;
int greenTime = 5000000;
int yellowTime = 2000000;
int timeAfterYellow = 1000000;
int blinkTime = 500000;
int c = 0;
int t = 0;

uint32_t swBig = 0x00;
uint32_t swBig_old = 0x00;
uint32_t swSmall = 0x00;
uint32_t swSmall_old = 0x00;

int main(){
	PortAF_Init();
	while(1){
		SwitchInput();
		selectMode();
	}	
	return 0;
}

void selectMode(void){
	// big switch for ON and OFF, small switch for emergency mode
	if (state_big == 1){
		if (state_small != 1){
			normalMode();
		}
		else{
			emergencyMode();
		}
	}
	else{
		closeSystem();
		state_small = 0;
	}
}

void PortAF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x21; //activate clock for part A, port F
	
	// initialize port A pins 2,3,4,5,6, north-bound
	GPIO_PORTA_DIR_R &= ~0x60; // external switches will be used as input to PA5, PA6 pins(0110 0000=0x60)
	GPIO_PORTA_DIR_R |= 0x1C;  // set PA2, PA3, PA4 as output so PA2=1,PA3=1, PA4=1 (0001 1100 = 0x1C)
	GPIO_PORTA_DEN_R |= 0x7C;  //enable digital function at 2,3,4,5,6 pins
	
	// initialize port F pins 1,2,3, east-bound
	GPIO_PORTF_DIR_R |= 0x0E;//Pin directions for Port F pins 1,2,3
	GPIO_PORTF_DEN_R |= 0x0E;//Enable digital mode operation GPIO_DEN for pins 1,2,3
}

void SwitchInput(void){
	uint32_t data = GPIO_PORTA_DATA_R & 0x60;// access port A pins 5,6 as inputs
	swBig = data&0x40;                      // access port A pin 6, big switch
	swSmall = data&0x20;                    // access port A pin 5, small switch
	if (swBig==0x40 && swBig_old==0x00){
		state_big = 1 - state_big;
	}
	if (swBig==0x00 && swBig_old==0x40){
		delay(5000);
	}
	swBig_old = swBig;
	if (state_big==1){                            // when big switch is ON (the system is turned on), 
		if (swSmall==0x20 && swSmall_old==0x00){    // small switch can work
			state_small = 1 - state_small;
		}
		if (swSmall==0x00 && swSmall_old==0x20){
			delay(5000);
		}
		swSmall_old = swSmall;
	}
}

void normalMode(void){
	if(c<1){
		GPIO_PORTA_DATA_R = 0x04;    // north-bound RED LED ON
		GPIO_PORTF_DATA_R = 0x04;    // east-bound YELLOW LED ON, north-bound RED LED no change
		delay(yellowTime);
		c++;
	}
	else if(c<2){
		GPIO_PORTA_DATA_R = 0x04;  // north-bound RED LED ON
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON, wait for some time
		delay(timeAfterYellow);
		c++;
	}
	else if(c<3){
		GPIO_PORTA_DATA_R = 0x10;    // north-bound GREEN LED ON
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON
		delay(greenTime);
		c++;
	}
	else if(c<4){
		GPIO_PORTA_DATA_R = 0x08;    // north-bound YELLOW LED ON
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON
		delay(yellowTime);
		c++;
	}
	else if(c<5){
		GPIO_PORTA_DATA_R = 0x04;    // north-bound RED LED ON, wait for some time
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON
		delay(timeAfterYellow);
		c++;
	}
	else if(c<6){
		GPIO_PORTA_DATA_R = 0x04;    // north-bound RED LED ON
		GPIO_PORTF_DATA_R = 0x08;    // east-bound GREEN LED ON
		delay(greenTime);
		c++;
	}
	else{
		c=0;
	}
}

void closeSystem(void){
	GPIO_PORTA_DATA_R &= ~0x1C;  // shutdown north-bound LEDs
	GPIO_PORTF_DATA_R &= ~0x0E;  // shutdown east-bound LEDs	
}

void emergencyMode(void){	
	if (t<1){
		GPIO_PORTA_DATA_R = 0x08;  // north-bound yellow LED ON
		GPIO_PORTF_DATA_R = 0x02;  // east-bound red LED ON		
		delay(blinkTime);
		t++;
	}
	else if (t<2){
		GPIO_PORTA_DATA_R = 0x00;  // north-bound yellow LED OFF
		GPIO_PORTF_DATA_R = 0x00;  // east-bound red LED OFF
		delay(blinkTime);
		t++;
	}
	else{
		t=0;
	}
}

void delay (int tm){
  int cnt =0;
  while (cnt<tm)
		  ++cnt;
}

