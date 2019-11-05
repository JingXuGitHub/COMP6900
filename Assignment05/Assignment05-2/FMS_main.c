#include "tm4c123gh6pm.h"
#include <stdint.h>

void SysTick_Init(void);
void SysTick_WaitSec(uint32_t delayInSec);
void PortE_Init(void);  
void PortA_Init(void);
uint32_t PortE_Input(void);

struct State{
	unsigned long Out;
	unsigned long Time;
	const struct State *Next[4];
};
typedef const struct State STyp;
#define Idle &FSM[0]
#define Ready &FSM[1]
#define Standby &FSM[2]
STyp FSM[3]={
	{0x04, 1, {Idle, Idle, Ready, Idle}},
	{0x08, 1, {Ready, Idle, Ready, Standby}},
	{0x10, 1, {Standby, Idle, Standby, Standby}},
};

int main(void){
	STyp *Pt;   // state pointer
	static unsigned long Input;	
	PortA_Init();
	PortE_Init();
	SysTick_Init();	
	Pt = Idle;
	while(1){
		GPIO_PORTA_DATA_R = Pt->Out;  // set output
		SysTick_WaitSec(Pt->Time);    // wait some time
		Input = PortE_Input();        // read buttons
		Pt = Pt->Next[Input];         // go to next state
	}
	return 0;
}

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;  // 1) disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;  //2) maximum reload value
	NVIC_ST_CURRENT_R = 0;  // 3) any wirte to current clears it
	NVIC_ST_CTRL_R = 0x00000005;  //4) enable SysTick with core clock
}

void SysTick_Wait(uint32_t delay){
	NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
	NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
	while((NVIC_ST_CTRL_R&0x00010000)==0){};  // wait for count flag
}

void SysTick_WaitSec(uint32_t delayInSec){
	uint32_t i;
	for(i=0;i<delayInSec*5;i++){  
		SysTick_Wait(3200000);  // wait 200ms == 3200000 counts
	}
}

void PortA_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x01; //activate clock for part A
	// initialize port A pins 2,3,4,5,6, north-bound
	GPIO_PORTA_DIR_R |= 0x1C;  // set PA2, PA3, PA4 as output so PA2=1,PA3=1, PA4=1 (0001 1100 = 0x1C)
	GPIO_PORTA_DEN_R |= 0x1C;  //enable digital function at 2,3,4 pins
}

void PortE_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10;  //Activate clock for port E - fifth bit
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x1F;
	GPIO_PORTE_DIR_R &= ~0x03; //set pins 0,1 as input pins (set bits one)
	//GPIO_PORTE_PUR_R = 0x03; 
	GPIO_PORTE_DEN_R |= 0x03;  //enable digital function at 0,1 pins
}

uint32_t PortE_Input(void){
	uint32_t data = GPIO_PORTE_DATA_R & 0x13;
	return(data); 
}
