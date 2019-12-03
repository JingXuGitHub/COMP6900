#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "MatrixKeyPad.h"

void SysTick_Init(void);
void SysTick_Wait200ms(uint32_t delayIn200ms);
void PortBE_Init(void);
void PortF_Init(void);
void ScanKeyPad(void);

char keypressed;
char LastKey;

struct State{
	uint32_t Out[10];
	double Time;
	const struct State *Next[5];
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
	Output data structure: the first five data show the current state the system is, the last five data indicate what the user should do.
	Input: 5(input 5 cents), 10(input 10 cents), 25(input 25 cents), Select(select soda button), Return(return coin button) 
	Output: ten green LEDs indicate the state. Red LED: need more money; Blue LED: money is enough; 
			    Yellow LED: take the changes.
	At each state,
	When input 5, output Out[0] and Out[5];
	When input 10, output Out[1] and Out[6];
	When input 25, output Out[2] and Out[7];
	When input Select, output Out[3] and Out[8];
	When input Select, output Out[4] and Out[9];
	*/
	{{0x02,0x04,0x20,0x01,0x01,0x02,0x02,0x08,0x02,0x02}, 3, {S5, S10, S25, S0, S0}},  // S0 state, input is 5, goto S5;
																					             // input 10,goto S10; input 25, goto S25;
																											 // input Select,goto S0; input Return, goto S0;
	
	{{0x04,0x08,0x80,0x02,0x01,0x02,0x02,0x08,0x02,0x02}, 3, {S10, S15, S30, S5, S0}}, // S5 state, input is 5, goto S10;
																					             // input 10,goto S15; input 25, goto S30;
																											 // input Select,goto S5; input Return, goto S0;
	
	{{0x08,0x10,0x80,0x04,0x01,0x02,0x02,0x08,0x02,0x02}, 3, {S15, S20, S35, S10, S0}}, // S10 state, input is 5, goto S15;
																					             // input 10,goto S25; input 25, goto S35;
																											 // input Select,goto S10; input Return, goto S0;
	
	{{0x10,0x20,0x100,0x08,0x01,0x02,0x08,0x08,0x02,0x02}, 3, {S20, S25, S40, S15, S0}}, // S15 state, input is 5, goto S20;
																					             // input 10,goto S25; input 25, goto S40;
																											 // input Select,goto S15; input Return, goto S0;
	
	{{0x20,0x40,0x200,0x10,0x01,0x08,0x08,0x08,0x02,0x02}, 3, {S25, S30, S45, S20, S0}}, // S20 state, input is 5, goto S25;
																					             // input 10,goto S30; input 25, goto S45;
																											 // input Select,goto S20; input Return, goto S0;
	
	{{0x20,0x20,0x20,0x01,0x01,0x0C,0x0C,0x0C,0x02,0x02}, 3, {S25, S25, S25, S0, S0}}, // 25 cents state, input 5,10,25, goto S25;
																											 // input Select, goto S0; input Return, goto S0;
	
	{{0x40,0x40,0x40,0x01,0x01,0x0C,0x0C,0x0C,0x04,0x02}, 3, {S30, S30, S30, S0, S0}}, // 30 cents state, input 5,10,25, goto S30;
																											 // input Select, goto S0; input Return, goto S0;
																					
	{{0x80,0x80,0x80,0x01,0x01,0x0C,0x0C,0x0C,0x04,0x02}, 3, {S35, S35, S35, S0, S0}}, // 35 cents state, input 5,10,25, goto S35;
																											 // input Select, goto S0; input Return, goto S0;
																					
	{{0x100,0x100,0x100,0x01,0x01,0x0C,0x0C,0x0C,0x04,0x02}, 3, {S40, S40, S40, S0, S0}}, // 40 cents state, input 5,10,25, goto S40;
																											 // input Select, goto S0; input Return, goto S0;
																					
	{{0x200,0x200,0x200,0x01,0x01,0x0C,0x0C,0x0C,0x04,0x02}, 3, {S45, S45, S45, S0, S0}}, // 45 cents state, input 5,10,25, goto S4;
																											 // input Select, goto S0; input Return, goto S0;
																					
};

int main(void){
	STyp *Pt;   // state pointer	
	PortBE_Init();
	Matrix_Init();
	PortF_Init();
	SysTick_Init();	
	Pt = S0;                   // initialize state
	GPIO_PORTF_DATA_R = 0x02;  // initialize PORT F
	GPIO_PORTB_DATA_R = 0x01;  // initialize PORT B
	while(1){
		ScanKeyPad();  // read keypad
		if(keypressed){
			if(keypressed == '1'){           // when input 5, output Out[0] and Out[5], state go to Next[0]
					if((Pt->Out[0]) <= 0xFF){
						GPIO_PORTE_DATA_R &= ~0x03;
						GPIO_PORTB_DATA_R = Pt->Out[0]; 
					}else{
						GPIO_PORTB_DATA_R = 0x00;
						GPIO_PORTE_DATA_R = (Pt->Out[0]>>8)&0x03;
					}	
					GPIO_PORTF_DATA_R = Pt->Out[5];
					SysTick_Wait200ms(Pt->Time);    
					Pt = Pt->Next[0]; 
					keypressed = ' ';
			}        
			else if(keypressed == '2'){      // when input 10, output Out[1] and Out[6], state go to Next[1]
					if((Pt->Out[1]) <= 0xFF){
						GPIO_PORTE_DATA_R &= ~0x03;
						GPIO_PORTB_DATA_R = Pt->Out[1]; 
					}else{
						GPIO_PORTB_DATA_R = 0x00;
						GPIO_PORTE_DATA_R = (Pt->Out[1]>>8)&0x03;
					}
					GPIO_PORTF_DATA_R = Pt->Out[6];					
					SysTick_Wait200ms(Pt->Time);    
					Pt = Pt->Next[1]; 
					keypressed = ' ';
			}				
			else if(keypressed == '3'){      // when input 25, output Out[2] and Out[7], state go to Next[2]
					if((Pt->Out[2]) <= 0xFF){
						GPIO_PORTE_DATA_R &= ~0x03;
						GPIO_PORTB_DATA_R = Pt->Out[2]; 
					}else{
						GPIO_PORTB_DATA_R = 0x00;
						GPIO_PORTE_DATA_R = (Pt->Out[2]>>8)&0x03;
					}
					GPIO_PORTF_DATA_R = Pt->Out[7];
					SysTick_Wait200ms(Pt->Time);    
					Pt = Pt->Next[2];
					keypressed = ' ';
			}
			else if(keypressed == 'A'){      // when input Select, output Out[3] and Out[8], state go to Next[3]
					if((Pt->Out[3]) <= 0xFF){
						GPIO_PORTE_DATA_R &= ~0x03;
						GPIO_PORTB_DATA_R = Pt->Out[3]; 
					}else{
						GPIO_PORTB_DATA_R = 0x00;
						GPIO_PORTE_DATA_R = (Pt->Out[3]>>8)&0x03;
					}			 
					GPIO_PORTF_DATA_R = Pt->Out[8];
					SysTick_Wait200ms(Pt->Time);    
					Pt = Pt->Next[3];
					keypressed = ' ';
			}
			else if(keypressed == 'B'){      // when input Return, output Out[4] and Out[9], state go to Next[4]
					if((Pt->Out[4]) <= 0xFF){
						GPIO_PORTE_DATA_R &= ~0x03;
						GPIO_PORTB_DATA_R = Pt->Out[4]; 
					}else{
						GPIO_PORTB_DATA_R = 0x00;
						GPIO_PORTE_DATA_R = (Pt->Out[4]>>8)&0x03;
					}			 
					GPIO_PORTF_DATA_R = Pt->Out[9];
					SysTick_Wait200ms(Pt->Time);    
					Pt = Pt->Next[4];
					keypressed = ' ';
			}
		}
	}
	return 0;
}

void ScanKeyPad(void){  
	char thisKey; 
	int32_t n;
  thisKey = MatrixKeypad_Scan(&n); // scan keypad
  if((thisKey != LastKey) && (n == 1)){
		keypressed = thisKey;
    LastKey = thisKey;
  } else{
    LastKey = 0; 
  }
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

void SysTick_Wait200ms(uint32_t delayIn200ms){
	uint32_t i;
	for(i=0;i<delayIn200ms;i++){  
		SysTick_Wait(3200000);  // wait 200ms == 3200000 counts
	}
}

void PortBE_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x12;  //Activate clock for port B, C - 0001 0010
	// initialize port B
	GPIO_PORTB_DIR_R |= 0xFF; //set all pins in port B as output pins (set bits ones)
	GPIO_PORTB_ODR_R = 0;
	GPIO_PORTB_PCTL_R = 0x00000000;
  GPIO_PORTB_AFSEL_R = 0;     // disable alternate functionality on PB
  GPIO_PORTB_AMSEL_R = 0;     // disable analog functionality on PB
	GPIO_PORTB_DEN_R |= 0xFF;  //enable digital function for all pins
  
	// initialize port E
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x1F;
	GPIO_PORTE_DIR_R |= 0x03; //set pins 0,1 as output pins (set bits zeros)
	//GPIO_PORTE_PUR_R = 0x03; 
	GPIO_PORTE_DEN_R |= 0x03;  //enable digital function at 0,1 pins
	
}

void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20; //activate clock for port F
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R |= 0x1F;
	// initialize port F pins 1,2,3
	GPIO_PORTF_DIR_R |= 0x0E;//Pin directions for Port F pins 1,2,3, as output
	GPIO_PORTF_DEN_R |= 0x0E;//Enable digital mode operation GPIO_DEN for pins 1,2,3
}

