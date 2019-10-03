			GLOBAL main
			AREA  mycode, CODE, READONLY

varX		EQU   0x35                 	; define constant with the value of 0x35
			DCD   varX			; store varX into memory
			
			ENTRY
main  		BL	myFunc			; branch to myFunc
			MOV R3, #0x333			; put 0x333 into R3				
			
myFunc 		LDR R0, =varX                 	; uses R0 to point to the address of varX variable
			MOV R0, R0, LSR #2		; right shift 2 bits R0
			MOV R1, #0x15			; put #0x15 into R1
			ADD R2, R0, R1			; add R1 to R0, then put the result to R2
			BX	R14

Stayhere	B	Stayhere	
			END
