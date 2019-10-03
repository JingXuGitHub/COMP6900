			GLOBAL main
			AREA  mycode, CODE, READONLY

N			EQU   0x11           	; define variable N
				
			ENTRY
main  		MOV 	R0, #N			; move N into R0
			MOV  	R1, #200		; move literal 200 into R1
			MUL 	R1, R1, R0		; multiple R1 and R0, put the result into R1
			MOV	R2, #30			; move literal 30 into R2
			ADD 	R2, R2, R1		; add R1 to R2, then put the result into R2	
			MOV	R3, #10			; move literal 10 into R3
			UDIV 	R3, R2, R3		; divide R2 by R3, put the result into R3

Stayhere	B	Stayhere	
			END
