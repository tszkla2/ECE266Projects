; Buzzer driver functions built on top of Tivaware
; Lab 4, ECE 266, spring 2018
; Created by Zhao Zhang (edited Jonathan Wacker)

; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/adc.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "ranger.h"
 
 
					.text
;					.global motionOn
;					.global motionOff
 

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, andthen the buzzer's
; signal pin is connected to PC4. 
RANGER_PERIPH   .field  SYSCTL_PERIPH_WTIMER0
RANGER_PORT     .field 	GPIO_PORTC_BASE
RANGER_PIN      .equ	GPIO_PIN_5
RANGER_Trig		.field  ADC_TRIGGER_PROCESSOR

;
; void rangerInit(): Initialze the rotary peripheral, port, and pin direction
;
rangerInit      PUSH 	{LR}
				
				; Initialize the GPIO peripheral for the port that the rotary uses:
                ;   Call SysCtlPeripheralEnable(RANGER_PERIPH)
                LDR   	r0, RANGER_PERIPH
                BL    	SysCtlPeripheralEnable
				
				; Preip
loop			LDR  	r0, RANGER_PERIPH
				BL   	SysCtlPeripheralReady
				TEQ  	r0, #1
				BNE  	loop

				; Configure ADC sequencer #0
				LDR	  	r0, ROTARY_PORT
				MOV   	r1, #0
				LDR  	r2, ROTARY_Trig
				MOV   	r3, #0
				BL    	TimerConfigure
				
				; Configure event
				LDR	  	r0, ROTARY_PORT
				MOV   	r1, #0
				LDR   	r2, ROTARY_Trig
				MOV   	r3, #0
				LDR   	r4, TIMER_EVENT_POS_EDGE
				MOV   	r5, #0
				BL	  	TimerControlEvent
				
				; Time Trig configure
				LDR	  	r0, ROTARY_PORT
				MOV   	r1, #0
				LDR  	r2, ROTARY_Trig
				MOV   	r3, #0
				BL    	TimerEnable
				
				; Interpreting timer status
				LDR	  	r0, ROTARY_PORT
				MOV   	r1, #0
				LDR  	r2, #false
				MOV   	r3, #0
				BL		TimerIntStatus
				
				; Configure a single step of the sequencer
loop_two		MOV     r0, #100
				BX		waitUS			; Time delay for 100 milsec
				LDR	  	r0, ROTARY_PORT
				MOV   	r1, #0
				LDR	  	r0, RANGER_Trig
				MOV   	r1, #0
				BL    	TimerIntClear
				TEQ  	r0, #1
				BNE  	loop_two

                POP   	{PC}
				

rangerGet   	PUSH  	{LR}		; save return address
				
				; Trigger
				LDR   	r0, ROTARY_PORT
				MOV   	r1, #0
				BL	  	ADCProcessorTrigger
				
				; Wait until the sample sequence has completed
loop_three		LDR	  	r0, ROTARY_PORT
				MOV	 	r1, #0
				MOV   	r2, #0
				BL    	ADCIntStatus
				CMP  	r0, #0
				BEQ  	loop_three
				
				LDR   	r0, ROTARY_PORT
				MOV   	r1, #0
				SUB   	SP, #4
				MOV   	r2, sp ; addr of adcReading is sp
				BL    	ADCSequenceDataGet

				POP 	{r0, pc} ; pop adcReading to r0 and return