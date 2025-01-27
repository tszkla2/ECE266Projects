; Buzzer driver functions built on top of Tivaware
; Lab 4, ECE 266, spring 2018
; Created by Zhao Zhang (edited Jonathan Wacker)

; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "pwmBuzzer.h", "driverlib/timer.h"


					.text
;					.global motionOn
;					.global motionOff


; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, andthen the buzzer's
; signal pin is connected to PC4.
BUZZER_PERIPH   .field  SYSCTL_PERIPH_WTIMER0
BUZZER_GPIO		.field  SYSCTL_PERIPH_GPIOC
BUZZER_TIME		.field  WTIMER0_BASE
BUZZER_BASE     .field 	GPIO_PORTC_BASE
BUZZER_PINM   	.field 	GPIO_PC4_WT0CCP0
BUZZER_PIN      .equ	GPIO_PIN_4



;
; void rangerInit(): Initialze the rotary peripheral, port, and pin direction
;
buzzerInit      PUSH 	{LR}

				; 	Initialize the GPIO peripheral for the port that the ranger uses:
				LDR   	r0, BUZZER_GPIO
                BL    	SysCtlPeripheralEnable

                LDR   	r0, BUZZER_PERIPH
                BL    	SysCtlPeripheralEnable

				LDR 	r0, BUZZER_BASE
				MOV 	r1, #BUZZER_PIN
				BL		GPIOPinTypeTimer

				LDR 	r0, BUZZER_PINM
				BL		GPIOPinConfigure

				;	Time configuring
				LDR		r0, BUZZER_TIME
				MOV 	r1, #TIMER_CFG_SPLIT_PAIR
				ORR 	r1, #TIMER_CFG_A_CAP_TIME_UP
				BL 		TimerConfigure

				;	Time edge trigger
				LDR		r0, BUZZER_TIME
				MOV 	r1, #TIMER_A
				MOV		r2, #10
				BL		TimerLoadSet

				LDR		r0, BUZZER_TIME
				MOV 	r1, #TIMER_A
				MOV		r2, #9


				;	Activates timer
				LDR		r0, BUZZER_TIME
				MOV 	r1, #TIMER_A
				BL		TimerEnable

                POP   	{PC}



buzzerOn	   	PUSH  	{LR, r0}		; save return address

				MOV     r3, r0

				LDR		r0, BUZZER_TIME
				MOV		r1, #TIMER_A
				MOV 	r2,	r3
				BL		TimerLoadSet

				MOV		r3,	r3, LSR #32

				LDR		r0, BUZZER_TIME
				MOV		r1, #TIMER_A
				MOV 	r2, r3  ;PERIOD - CYCLE
				BL		TimerMatchSet

				POP 	{pc} ; pop final value to r0 and return
