#include "tm4c123gh6pm.h"
#include <stdint.h>

void SysTick_Init(void);
void SysTick_WaitSec(uint32_t delayInSec);
void PortE_Init(void);  
void PortA_Init(void);
void PortF_Init(void);
uint32_t PortEF_Input(void);
static uint32_t data;
static unsigned long Input;	

struct State{
	unsigned long Out[4];
	unsigned long Time;
	const struct State *Next[4];
};
typedef const struct State STyp;
#define S0 &FSM[0]
#define S5 &FSM[1]
#define S10 &FSM[2]
#define S15 &FSM[3]
#define S20 &FSM[4]
#define S25 &FSM[5]
#define S30 &FSM[6]
#define S35 &FSM[7]
#define S40 &FSM[8]
#define S45 &FSM[9]
STyp FSM[10]={
	/*
	output red light: need more money; output green light: money is enough, you can take the soda drink; output yellow light: don't forget to take the changes.
	*/
	{{0x04,0x04,0x10,0x04}, 1, {S5, S10, S25,S0}},  // 0 cents state, when input is 0x01(5 cents), output 0x04(red light on);
																					// input 0x02,output 0x04(red light on); input 0x04,output 0x10(green light on)
	
	{{0x04,0x04,0x18,0x04}, 1, {S10, S15, S30,S0}}, // 5 cents state, when input is 0x01(5 cents), output 0x04(red light on);
																					// input 0x02,output 0x04(red light on); input 0x04,output 0x18(green and yellow lights on, changes)
	
	{{0x04,0x04,0x18,0x04}, 1, {S15, S20, S35,S0}}, // 10 cents state, when input is 0x01(5 cents), output 0x04(red light on);
																					// input 0x02,output 0x04(red light on); input 0x04,output 0x18((green and yellow lights on, changes)
	
	{{0x04,0x10,0x18,0x04}, 1, {S20, S25, S40,S0}}, // 15 cents state, when input is 0x01(5 cents), output 0x04(red light on);
																					// input 0x02,output 0x08(green light on); input 0x04,output 0x18((green and yellow lights on, changes)
	
	{{0x10,0x18,0x18,0x04}, 1, {S25, S30, S45,S0}}, // 20 cents state, when input is 0x01(5 cents), output 0x10(green light on);
																					// input 0x02,output 0x18((green and yellow lights on, changes); input 0x04,output 0x18((green and yellow lights on, changes)
	
	{{0x18,0x18,0x18,0x04}, 1, {S25, S25, S25,S0}}, // 25 cents state, no matter what the input is, output 0x18(green and yellow lights on, changes)
	
	{{0x18,0x18,0x18,0x04}, 1, {S30, S30, S30,S0}}, // 30 cents state, no matter what the input is, output 0x18(green and yellow lights on, changes)
																					
	{{0x18,0x18,0x18,0x04}, 1, {S35, S35, S35,S0}}, // 35 cents state, no matter what the input is, output 0x18(green and yellow lights on, changes)
																					
	{{0x18,0x18,0x18,0x04}, 1, {S40, S40, S40,S0}}, // 40 cents state, no matter what the input is, output 0x18(green and yellow lights on, changes)
																					
	{{0x18,0x18,0x18,0x04}, 1, {S45, S45, S45,S0}}, // 45 cents state, no matter what the input is, output 0x18(green and yellow lights on, changes)
																					
};

int main(void){
	STyp *Pt;   // state pointer
	
	PortA_Init();
	PortE_Init();
	PortF_Init();
	SysTick_Init();	
	Pt = S0;
	while(1){
		Input = PortEF_Input();  // read buttons
		if(Input != 0x00){
			if(Input == 0x01){           // when input is 0x01 (5 cents), output Out[0], state go to Next[0]
					GPIO_PORTA_DATA_R = Pt->Out[0];  
					SysTick_WaitSec(Pt->Time);    
					Pt = Pt->Next[0]; 
			}        
			else if(Input == 0x02){      // when input is 0x02 (10 cents), output Out[1], state go to Next[1]
					GPIO_PORTA_DATA_R = Pt->Out[1];  
					SysTick_WaitSec(Pt->Time);    
					Pt = Pt->Next[1]; 
			}				
			else if(Input == 0x04){      // when input is 0x04 (25 cents), output Out[2], state go to Next[2]
					GPIO_PORTA_DATA_R = Pt->Out[2];  
					SysTick_WaitSec(Pt->Time);    
					Pt = Pt->Next[2];
			}
		}
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
	GPIO_PORTA_DIR_R |= 0x1C;  // set PA2, PA3, PA4 as output so PA2=1,PA3=1, PA4=1 (0001 1100 = 0x1C)
	GPIO_PORTA_DEN_R |= 0x1C;  //enable digital function at 2,3,4 pins
}

void PortE_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10;  //Activate clock for port E - fifth bit
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x1F;
	GPIO_PORTE_DIR_R &= ~0x03; //set pins 0,1 as input pins (set bits zeros)
	//GPIO_PORTE_PUR_R = 0x03; 
	GPIO_PORTE_DEN_R |= 0x03;  //enable digital function at 0,1 pins
}

void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20; //activate clock for port F
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R |= 0x1F;
	// initialize port F pins 1,4
	GPIO_PORTF_DIR_R &= ~0x11;//Pin directions for Port F pins 1,4, as input
	GPIO_PORTF_PUR_R = 0x11; 
	GPIO_PORTF_DEN_R |= 0x11;//Enable digital mode operation GPIO_DEN for pins 1,4
}

uint32_t PortEF_Input(void){
	/* output the input from port E and port F, data from port E left-shift 2 bits, 
		and do | operation with dataF, so data format is bit7, bit6, bit5, sw1, bit3, bigSW, smallSW, sw2.*/
	uint32_t dataE = GPIO_PORTE_DATA_R & 0x03;
	uint32_t dataF = GPIO_PORTF_DATA_R & 0x11;
	dataF ^= 0x11;
	data = (dataE<<1)|dataF;
	return(data); 
}
