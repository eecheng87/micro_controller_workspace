LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    start:
	movlw B'11010111'
	movwf 0x00 ; move first operand to f
	movlw B'10011110'
	andwf 0x00,1 ; store result back to f(0x00)
	movlw 0xffff
	xorwf 0x00,0 ; store answer to WREG
	movwf LATA
	
	movlw B'01010011'
	movwf 0x00
	movlw B'11100100'
	iorwf 0x00,1
	movlw 0xffff
	xorwf 0x00,0
	movwf LATB
	nop
    end


