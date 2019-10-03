
// Define the memory maps based on addresses given in page 90 of the data sheet
// A macro is a fragment of code that is given a name. 
// You can define a macro in C using the #define preprocessor directive.
// It improves readability of your code.


// #include "tm4c123gh6pm.h"
#include <stdio.h>

#define ENCLOCK  (*((unsigned int *) 0x400FE608))
#define PIN_DIRECT (*((unsigned int *) 0x40025400))
#define DIGITAL_ENABLE 	*((unsigned int *) 0x4002551C)
#define ENABLE_PIN_F *((unsigned int *) 0x400253FC)

void delay (int tm);

int main(){
  // LED blinking, PIN F1, F2, F3 are connected to RGB onboard LEDs
  
  // Onboard LEDs are mapped at pins 1, 2, 3 of PORT F - so we need to program port F to blink the onboard LEDs
  
  ENCLOCK = 0x20; // 0010 0000, activating the sixth bit to Enable clock to GPIO Port F in Run mode, 
										  // See page 229/315 for this particular memory address
  
  PIN_DIRECT = 0x0E; // 0000 1110, PIN directions Set the 2nd, 3rd, 4th bits (Pins 1, 2, 3 of Port F), 
											// Set = 1, output pins, Reset = 0, input pins, See page 624
  DIGITAL_ENABLE = 0x0E; // 0000 1110, enable digital function for the pins, see page 643-644
  
  while (1){
	  ENABLE_PIN_F |= 0x02;//0000 0010, TRUN ON PF1 or pin 1 (Red LED) of Port F, offset 0x3FC denotes all pins are accessible  
	                                         // See table 6.3 Valvano

	  delay(10000000);
	  
		//ENABLE_PIN_F = 0x00;//0000 0000, TRUN OFF PF1, RED LED 
	  
	  //delay(10000000);
	  
		ENABLE_PIN_F |= 0x04;
	  
	  delay(10000000);
	  
	  //ENABLE_PIN_F = 0x00;
	  
	  //delay(10000000);
	  
	  ENABLE_PIN_F |= 0x08;
	  
	  delay(10000000);
	  
		//ENABLE_PIN_F = 0x00;
	  
	  //delay(10000000);
	                                                      
  }
	  									 
  return 0;

}

// In-class test
// 1. Use the provided header file of all memory definitions to clean and update your code
// 2. Observe and discuss the effect of friendly and unfriendly coding for each memory assignment
// For example, when you decide to turn the red LED ON by setting the corresponding bit, 
// you also want to reset other bits (for blue and green) and it's not the case that other bits need to be unaffected. 
// You final submission should use the header file definitions, an InitF() function, compact and efficient
// representations of the while statement. 
void delay (int tm){
  int cnt =0;
	  
	  while (cnt<tm)
		  ++cnt;

}
