#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <time.h>

void delay(int tm);
void PortA_Init(void);
void LED_Init(void);
void blinkLED_n(int n);
void SysTick_Init(void);
void LED_On(void);
void LED_Off(void);

int c = 0;

int main(){
	int blink_times = 5;
	static uint32_t before=0x00;
	static uint32_t elapsed=0x00;
	static int PF2_Data;
	LED_Init();
	PortA_Init();
	SysTick_Init();
	while(1){
		before = NVIC_ST_CURRENT_R;
		LED_On();
		PF2_Data = GPIO_PORTF_DATA_R & 0x04;
		blinkLED_n(blink_times);
		LED_Off();
		PF2_Data = GPIO_PORTF_DATA_R & 0x04;
		elapsed = (before-NVIC_ST_CURRENT_R)&0x00FFFFFF;
	}
	return 0;
}

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;  // 1) disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;  //2) maximum reload value
	NVIC_ST_CURRENT_R = 0;  // 3) any wirte to current clears it
	NVIC_ST_CTRL_R = 0x00000005;  //4) enable SysTick with core clock
}

void PortA_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x01; //activate clock for part A
	
	// initialize port A pins 2,3,4,5,6, north-bound
	GPIO_PORTA_DIR_R |= 0x1C;  // set PA2, PA3, PA4 as output so PA2=1,PA3=1, PA4=1 (0001 1100 = 0x1C)
	GPIO_PORTA_DEN_R |= 0x1C;  //enable digital function at 2,3,4 pins
}

void LED_Init(void){
	//volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x20;  // 1) activate clock for Port F
	//delay = SYSCTL_RCGC2_R;  // allow time for clock to start
	GPIO_PORTF_AMSEL_R &= ~0x04; 
	GPIO_PORTF_PCTL_R &= ~0x00000F00;
	GPIO_PORTF_DIR_R |= 0x04;
	GPIO_PORTF_AFSEL_R &= ~0x04;
	GPIO_PORTF_DEN_R |= 0x04;
}

void LED_On(void){
	GPIO_PORTF_DATA_R = 0x04;
}
void LED_Off(void){
	GPIO_PORTF_DATA_R = 0x00;
}

void blinkLED_n(int n){
	int c =0;
	while(1){
		if(c<n){
			GPIO_PORTA_DATA_R = 0x10;
			GPIO_PORTA_DATA_R = 0x00;
			c++;
		}
		else if(c<2*n){
			GPIO_PORTA_DATA_R = 0x08;
			GPIO_PORTA_DATA_R = 0x00;
			c++;
		}
		else if(c<3*n){
			GPIO_PORTA_DATA_R = 0x04;
			GPIO_PORTA_DATA_R = 0x00;
			c++;
		}
		else {
			break;
		}
	}
}


