; Buzzer driver functions built on top of Tivaware
; Lab 4, ECE 266, spring 2018
; Created by Zhao Zhang (edited Jonathan Wacker)

; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "ranger.h", "driverlib/timer.h"
 
 
					.text
;					.global motionOn
;					.global motionOff
 

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, andthen the buzzer's
; signal pin is connected to PC4. 
RANGER_PERIPH   .field  SYSCTL_PERIPH_WTIMER0
RANGER_GPIO		.field  SYSCTL_PERIPH_GPIOC
RANGER_TIME		.field  WTIMER0_BASE
RANGER_BASE     .field 	GPIO_PORTC_BASE
RANGER_PINM   	.field 	GPIO_PC4_WT0CCP0
RANGER_PIN      .equ	GPIO_PIN_4



;
; void rangerInit(): Initialze the rotary peripheral, port, and pin direction
;
rangerInit      PUSH 	{LR}
				
				; 	Initialize the GPIO peripheral for the port that the ranger uses:
                LDR   	r0, RANGER_PERIPH
                BL    	SysCtlPeripheralEnable
				
				LDR   	r0, RANGER_GPIO
                BL    	SysCtlPeripheralEnable
				
				;	Time configuring
				LDR		r0, RANGER_TIME
				MOV 	r1, #TIMER_CFG_SPLIT_PAIR
				ORR 	r1, #TIMER_CFG_A_CAP_TIME_UP
				BL 		TimerConfigure
				
				;	Time edge trigger
				LDR		r0, RANGER_TIME
				MOV 	r1, #TIMER_A
				MOV		r2, #TIMER_EVENT_BOTH_EDGES
				BL		TimerControlEvent
				
				;	Activates timer
				LDR		r0, RANGER_TIME
				MOV 	r1, #TIMER_A
				BL		TimerEnable

                POP   	{PC}


rangerGet   	PUSH  	{LR}		; save return address
				
				; 	---Sending a starting pulse for analysis
				LDR		r0, RANGER_BASE
				MOV		r1, #RANGER_PIN
				BL		GPIOPinTypeGPIOOutput

				;	--low waits 2
				LDR		r0, RANGER_BASE
				MOV		r1, #RANGER_PIN
				MOV		r2, #0
				BL		GPIOPinWrite

				MOV		r0, #2
				BL 		waitUs
				
				;	--High waits 5
				LDR		r0, RANGER_BASE
				MOV		r1, #RANGER_PIN
				MOV		r2, #RANGER_PIN
				BL		GPIOPinWrite
				
				MOV		r0, #5
				BL 		waitUs
				
				;	--Restore to fall edge
				LDR		r0, RANGER_BASE
				MOV		r1, #RANGER_PIN
				MOV		r2, #0
				BL		GPIOPinWrite
				
				
				;	---Configure time to pin and work accordingly
				LDR 	r0, RANGER_BASE
				MOV 	r1, #RANGER_PIN
				BL		GPIOPinTypeTimer
				
				LDR		r0, RANGER_PINM
				BL		GPIOPinConfigure
				
				;	-Clearing any fake interrupts
				LDR 	r0, RANGER_TIME
				MOV 	r1, #TIMER_CAPA_EVENT
				BL		TimerIntClear
				
				
				;	---Obtaining rising and falling edge values
				;	--Looping and waiting for trigger of rising edge
while_One		LDR 	r0, RANGER_TIME
				MOV		r1, #false
				BL		TimerIntStatus
				CMP		r0, #false
				BEQ		while_One
				
				;	-Get value
				LDR 	r0, RANGER_TIME
				MOV 	r1, #TIMER_A
				BL		TimerValueGet
				
				; 	-Push value to variable
				PUSH 	{r0}
				
				;	-Clearing any fake interrupts
				LDR 	r0, RANGER_TIME
				MOV 	r1, #TIMER_CAPA_EVENT
				BL		TimerIntClear
				
				;	--Looping and waiting for trigger of falling edge
while_Two		LDR 	r0, RANGER_TIME
				MOV		r1, #false
				BL		TimerIntStatus
				CMP		r0, #false
				BEQ		while_Two
				
				;	-Get value
				LDR 	r0, RANGER_TIME
				MOV 	r1, #TIMER_A
				BL 		TimerValueGet
				
				; 	-Push value to variable
				PUSH 	{r0}
				
				;	-Clearing any fake interrupts
				LDR 	r0, RANGER_TIME
				MOV 	r1, #TIMER_CAPA_EVENT
				BL		TimerIntClear
				
				
				;	---Calculation
				POP		{r0, r1}
				SUB		r0, r0, r1

				POP 	{pc} ; pop final value to r0 and return
