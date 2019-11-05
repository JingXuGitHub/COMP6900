#include "tm4c123gh6pm.h"
#include <stdint.h>

void sw2_Input(void);
void PortF_Init(void); 
void delay(int tm); 
void printString(char* string);
unsigned char UART_InChar(void);
void UART_OutChar(unsigned char data);
void UART_Init(void);

int sw2 = 0;
uint32_t data = 0x00;
uint32_t data_old = 0x00;

int main(){
	// Initialize Port F such that pins 1,2,3 are outputs
	UART_Init();
	PortF_Init();
	while(1){
		printString("Enter r or g or b: \n\r");  
		unsigned char myChar = UART_InChar();  // read character from PC keyboard input -wait for user input
		UART_OutChar(myChar);  //print on putty console to see what the user typed
		printString("\n\r");   // new line on putty console

		// Mapping between UART input and GPIO output
		switch (myChar){
			case 'r':
				GPIO_PORTF_DATA_R = 0x02; // Red LED ON
				break;
			case 'b':
				GPIO_PORTF_DATA_R = 0x04; // BLUE LED ON
				break;
			case 'g':
				GPIO_PORTF_DATA_R = 0x08; // GREEN LED ON
				break;
			default:
				GPIO_PORTF_DATA_R = 0x00; // ALL LEDs OFF
		}
	}
	return 0;
}

void sw2_Input(void){
	data = GPIO_PORTF_DATA_R & 0x01;
	data ^= 0x01;
	if (data==0x01 && data_old==0x00){
		sw2 = 1 - sw2;
	}
	if (data==0x00 && data_old==0x01){
		delay(5000);
	}
	data_old=data;
	
}

//Transmit characters (byte-by-byte) to PC
void printString(char* string){
	while(*string){
		UART_OutChar(*(string++));
	}
}

unsigned char UART_InChar(void){
	while((UART0_FR_R & 0x0010) != 0){;  // wait until RXFE is 0
		sw2_Input();
		if (sw2==1){
			printString("Nashville, TN \n\r"); 
			sw2 = 1 - sw2;
			printString("Enter r or g or b: \n\r");  
		}
	}
	return ((unsigned char) (UART0_DR_R&0xFF));
}

void UART_OutChar (unsigned char data){
	while ((UART0_FR_R&0x0020) != 0);  // wait until TXFF is 0
	UART0_DR_R = data;
}

void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;  //Activate clock for port F - sixth bit
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R |= 0x1F;
	GPIO_PORTF_DIR_R = 0x0E; //set pins 1,2,3 as output pins (set bits one)
	GPIO_PORTF_PUR_R = 0x11; 
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DEN_R |= 0x1F;  //enable digital function at 0,1,2,3,4 pins
}

// 16 MHz 
void UART_Init(void){
	SYSCTL_RCGC1_R |= 0x0001;  //activate UART0
	SYSCTL_RCGC2_R |= 0x0001;  //activate port A
	UART0_CTL_R &= ~0x0001;    //disable UART
	UART0_IBRD_R = 104;	// IBRD=int(16 000 000 /(16*9600)) = int(104.1666)
	UART0_FBRD_R = 11;	// FBRD=int(0.1666 * 64 + 0.5)=11
	UART0_LCRH_R = 0x0070;  // 8-bit word length, enable FIFO
	UART0_CTL_R = 0x0301;   //enable RXE, TXE and UART
	GPIO_PORTA_AMSEL_R &= ~0x03;  //disable analog function on PA1-0
	GPIO_PORTA_AFSEL_R |= 0x03;   //enable alt function on PA1-0
	GPIO_PORTA_DEN_R |= 0x03;     //enable digital I/O on PA1-0
}

void delay (int tm){
  int cnt =0;
  while (cnt<tm)
		++cnt;
}
