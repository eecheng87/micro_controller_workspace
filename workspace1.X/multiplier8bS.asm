LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    ; implement 8-bits sign multiplier
	op1 set 0x00
	op2 set 0x01
	num1 equ 0xFF
	num2 equ 0X01

	start:
	movlw num1
	movwf op1,0
	movlw num2
	movwf op2,0
	
	movf op1,0,0
	mulwf op2,0
	
	btfsc op2,7,0 ; skip if clear
	subwf PRODH,1,0 ; op1 is negative, so subtract extra part 
	movf op2,0,0
	btfsc op1,7,0 ; skip if clear
	subwf PRODH,1,0
	    
    	nop
    
end


