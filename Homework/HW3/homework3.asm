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
				SUB r4, r0, r1
				MOV r0, r4
				BX		lr
				.endasmfunc

;****************************************************************
; int sub2(int a, int b, int c)
;   Return sub2(sub2(a, b), c)
;   Your solution must call sub2() function.
;****************************************************************
				.global sub3
sub3				.asmfunc
				PUSH {r2, lr}
				BL sub2
				POP {r1}
				BL sub2
				POP {lr}
				.endasmfunc

;****************************************************************
; void strcpy(char* dst, char* src)
;   Copy a string from src[] to dst[]. The dst[] buffer must be
;   large enough to hold the source string.
;****************************************************************
				.global mystrcpy
mystrcpy			.asmfunc
				LDR r1, =srcStr
				LDR r0, =dstStr
				LDRB r2, [r], #1
				STRB r2, [r0], #1
				CMP r2, #0
				BNE loop
				B stop
				.endasmfunc

;****************************************************************
; int calc(int x, int y, int z)
;	return x * y + z – x
;****************************************************************
				.global calc
calc				.asmfunc
				MUL r4, r0, r1
				ADD r5, r3, r4
				SUB r6, r5, r0
				MOV r6, r0

				BX		lr
				.endasmfunc
