; Buzzer driver functions built on top of Tivaware
; Lab 4, ECE 266, spring 2018
; Created by Zhao Zhang (edited Jonathan Wacker)

; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "rotary.h"
 
 
					.text
;					.global motionOn
;					.global motionOff
 

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, andthen the buzzer's
; signal pin is connected to PC4. 
ROTARY_PERIPH   .field  SYSCTL_PERIPH_ADC0
ROTARY_PORT     .field 	ADC0_BASE
ROTARY_PIN      .equ	GPIO_PIN_4

;
; void motionInit(): Initialze the motion peripheral, port, and pin direction
;
rotaryInit      PUSH 	{LR}
				
				; Initialize the GPIO peripheral for the port that the motion uses:
                ;   Call SysCtlPeripheralEnable(ROTARY_PERIPH)
                LDR   r0, ROTARY_PERIPH
                BL    SysCtlPeripheralEnable
				
				; Configure ADC sequencer #0
				LDR	  r0, ROTARY_PORT
				MOV   r1, #0
				LDR   r2, ADC_TRIGGER_PROCESSOR
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
				
;
; void motionActive(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
rotaryGet   	PUSH  {LR}		; save return address
				SUB sp, #4  	; allocate a word for adcReading
				
				; Trigger
				LDR   r0, ROTARY_PORT
				MOV   r1, #0
				BX	  ADCProcessorTrigger
				
				; Wait until the sample sequence has completed
while_loop		LDR	  r0, ROTARY_PORT
				MOV	  r1, #0
				MOV   r2, #false
				BX    ADCIntStatus
				CMP	  r0, #0
				BEQ   while_loop
				
				LDR   r0, ROTARY_PORT
				MOV   r1, #0
				MOV   r2, sp ; addr of adcReading is sp
				BL    ADCSequenceDataGet
				POP {r0, pc} ; pop adcReading to r0 and return