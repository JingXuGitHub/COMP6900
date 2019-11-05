#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <time.h>

void PortAF_Init(void);
void normalMode(void);
void SysTick_Init(void);
void LEDs_Data(void);

int c = 0;
int n = 0;

static uint32_t before=0x00;
static uint32_t elapsed=0x00;

static uint32_t northG = 0x00;
static uint32_t northY = 0x00;
static uint32_t northR = 0x00;

static uint32_t eastG = 0x00;
static uint32_t eastY = 0x00;
static uint32_t eastR = 0x00;

int main(){
	SysTick_Init();
	PortAF_Init();
	
	before = NVIC_ST_CURRENT_R;
	while(n<=10){
		normalMode();
		LEDs_Data();
	}	
	elapsed = (before-NVIC_ST_CURRENT_R)&0x00FFFFFF;
	
	return 0;
}

void LEDs_Data(void){
	northG = GPIO_PORTA_DATA_R & 0x10;
	northY = GPIO_PORTA_DATA_R & 0x08;
	northR = GPIO_PORTA_DATA_R & 0x04;
		
	eastG = GPIO_PORTF_DATA_R & 0x08;
	eastY = GPIO_PORTF_DATA_R & 0x04;
	eastR = GPIO_PORTF_DATA_R & 0x02;
}

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;  // 1) disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;  //2) maximum reload value
	NVIC_ST_CURRENT_R = 0;  // 3) any wirte to current clears it
	NVIC_ST_CTRL_R = 0x00000005;  //4) enable SysTick with core clock
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

void normalMode(void){
	if(c<1){
		GPIO_PORTA_DATA_R = 0x10;    // north-bound GREEN LED ON
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON
		n++;
		c++;
	}
	else if(c<2){
		GPIO_PORTA_DATA_R = 0x08;  // north-bound YELLOW LED ON
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON
		c++;
	}
	else if(c<3){
		GPIO_PORTA_DATA_R = 0x04;    // north-bound RED LED ON
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON
		c++;
	}
	else if(c<4){
		GPIO_PORTA_DATA_R = 0x04;    // north-bound RED LED ON
		GPIO_PORTF_DATA_R = 0x08;    // east-bound GREEN LED ON
		c++;
	}
	else if(c<5){
		GPIO_PORTA_DATA_R = 0x04;    // north-bound RED LED ON
		GPIO_PORTF_DATA_R = 0x04;    // east-bound YELLOW LED ON
		c++;
	}
	else if(c<6){
		GPIO_PORTA_DATA_R = 0x04;    // north-bound RED LED ON
		GPIO_PORTF_DATA_R = 0x02;    // east-bound RED LED ON
		c++;
	}
	else{
		c=0;
	}
}
