			GLOBAL main
			AREA  mycode, CODE, READONLY

Constant	EQU   0x23              ; define constant with the value of 0x23
			DCD   Constant          ; save the constant to the memory
P1			EQU   0x20000000        ; define address p1 with value of 0x20000000
	
			ENTRY

main  		MOV R0, #P1             ; move the value of P1 to R0
			MOV R1, #Constant	; move the value of Constant to R1
			STR R1, [R0]            ; store value of R1 into the memory location in R0

Stayhere  	B  Stayhere

			END
