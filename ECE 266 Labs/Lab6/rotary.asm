; Buzzer driver functions built on top of Tivaware
; Lab 4, ECE 266, spring 2018
; Created by Zhao Zhang (edited Jonathan Wacker)

; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "motion.h", "buzzer.h"
 
 
					.text
;					.global motionOn
;					.global motionOff
 

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, and then the buzzer's
; signal pin is connected to PC4.
MOTION_PERIPH   .field  SYSCTL_PERIPH_GPIOC
MOTION_PORT     .field 	GPIO_PORTC_BASE
MOTION_PIN      .equ	GPIO_PIN_5

;
; void motionInit(): Initialze the motion peripheral, port, and pin direction
;
rotaryInit      PUSH 	{LR}
				
				; Initialize the GPIO peripheral for the port that the motion uses:
                ;   Call SysCtlPeripheralEnable(ROTARY_PERIPH)
                LDR   r0, ROTARY_PERIPH
                BL    SysCtlPeripheralEnable

                ; Set the GPIO pin for the motion as input type:
                ;   Call GPIOPinTypeGPIOInput(ROTARY_PORT, ROTARY_PIN)
                LDR   r0, ROTARY_PORT
                MOV   r1, #ROTARY_PIN
                BL    GPIOPinTypeGPIOInput

                POP   {PC}
				
;
; void motionActive(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
rotarySamp  	PUSH  {LR}

                ; Write 1 to the GPIO pin that the buzzer uses:
				; Call GPIOPinWrite(ROTARY_PORT, ROTARY_PIN, ROTARY_PIN)
                LDR   r0, ROTARY_PORT
                MOV   r1, #ROTARY_PIN
                BL    GPIOPinRead

                POP   {PC}
