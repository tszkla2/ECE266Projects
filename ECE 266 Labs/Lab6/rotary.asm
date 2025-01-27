; Buzzer driver functions built on top of Tivaware
; Lab 4, ECE 266, spring 2018
; Created by Zhao Zhang (edited Jonathan Wacker)

; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/adc.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "rotary.h"
 
 
					.text
;					.global motionOn
;					.global motionOff
 

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, andthen the buzzer's
; signal pin is connected to PC4. 
ROTARY_PERIPH   .field  SYSCTL_PERIPH_ADC0
ROTARY_PORT     .field 	ADC0_BASE
ROTARY_PIN      .equ	GPIO_PIN_3
ROTARY_Trig		.field  ADC_TRIGGER_PROCESSOR

;
; void rotaryInit(): Initialze the rotary peripheral, port, and pin direction
;
rotaryInit      PUSH 	{LR}
				
				; Initialize the GPIO peripheral for the port that the rotary uses:
                ;   Call SysCtlPeripheralEnable(ROTARY_PERIPH)
                LDR   r0, ROTARY_PERIPH
                BL    SysCtlPeripheralEnable
				
				; Preip
loop			LDR  r0, ROTARY_PERIPH
				BL   SysCtlPeripheralReady
				TEQ  r0, #1
				BNE  loop

				; Configure ADC sequencer #0
				LDR	  r0, ROTARY_PORT
				MOV   r1, #0
				LDR   r2, ROTARY_Trig
				MOV   r3, #0
				BL    ADCSequenceConfigure
				
				; Configure a single step of the sequencer
				LDR	  r0, ROTARY_PORT
				MOV   r1, #0
				MOV   r2, #0
				MOV   r3, #(ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2)
				BL    ADCSequenceStepConfigure

                ; Set the GPIO pin for the motion as input type:
                ;   Call GPIOPinTypeGPIOInput(ROTARY_PORT, ROTARY_PIN)
                LDR   r0, ROTARY_PORT
                MOV   r1, #0
                BL    ADCSequenceEnable

                POP   {PC}
				

rotaryGet   	PUSH  {LR}		; save return address
				
				; Trigger
				LDR   r0, ROTARY_PORT
				MOV   r1, #0
				BL	  ADCProcessorTrigger
				
				; Wait until the sample sequence has completed
while_loop		LDR	  r0, ROTARY_PORT
				MOV	  r1, #0
				MOV   r2, #0
				BL    ADCIntStatus
				CMP  r0, #0
				BEQ  while_loop
				
				LDR   r0, ROTARY_PORT
				MOV   r1, #0
				SUB   SP, #4
				MOV   r2, sp ; addr of adcReading is sp
				BL    ADCSequenceDataGet

				POP {r0, pc} ; pop adcReading to r0 and return
