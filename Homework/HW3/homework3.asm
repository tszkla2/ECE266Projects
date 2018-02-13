;****************************************************************
; homework3.asm: Assembly code for homework 3 assignment.
;
; Created by Zhao Zhang for ECE 266 Spring 2018
;****************************************************************

; include the C declarations in homework3.h in this assembly program
			.cdecls "homework3.h"

			.text

;****************************************************************
; int sub2(int a, int b)
;   Return a-b
;****************************************************************
				.global sub2
sub2				.asmfunc


				BX		lr
				.endasmfunc

;****************************************************************
; int sub2(int a, int b, int c)
;   Return sub2(sub2(a, b), c)
;   Your solution must call sub2() function.
;****************************************************************
				.global sub3
sub3				.asmfunc


				BX		lr
				.endasmfunc

;****************************************************************
; void strcpy(char* dst, char* src)
;   Copy a string from src[] to dst[]. The dst[] buffer must be
;   large enough to hold the source string.
;****************************************************************
				.global sub3
mystrcpy			.asmfunc
				PUSH		{lr}


				POP		{pc}
				.endasmfunc

;****************************************************************
; int calc(int x, int y, int z)
;	return x * y + z – x
;****************************************************************
				.global calc
calc				.asmfunc


				BX		lr
				.endasmfunc
