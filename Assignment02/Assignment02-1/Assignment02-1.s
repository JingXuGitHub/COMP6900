      GLOBAL main
	  AREA  mycode, CODE, READONLY
	  
pInt  EQU    0x20000002  	; int *pInt = (int *) 0x2000002		

	  ENTRY

main  LDR R0, =pInt			; load pInt into R0
	  LDR R1, =0xBEEFFEED	; load 0xBEEFFEED into R1
	  STR R1, [R0]			; store value in R1 into memory address in R0, *pInt = 0xBEEFFEED
	  LDR R2, =0xABBCABBC	; load 0xABBCABBC into R2
	  STR R2, [R0,#4]		; store value in R2 into memory address in R0 increased by 4, *(pInt +1) = 0xABBCABBC
	  LDR R3, =0xCAADBAAD	; load 0xCAADBAAD into R3
	  STR R3, [R0,#16]		; store value in R3 into memory address in R0 increased by 16, *(pInt +4) = 0xCAADBAAD

stop  B  stop

      END