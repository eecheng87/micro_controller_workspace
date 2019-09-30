LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    start:
	movlw 0x04 ; index
	movwf 0x00,0
	movlw 0x0a ; A
	movwf TRISB
	movlw 0x03 ; B
	movwf TRISC
	clrf WREG
    loop:

	btfsc TRISC,0 ; skip if clear
	addwf TRISB,0 ; if 1, add A to wreg
	rrncf TRISC,1 ; rotate B
	rlncf TRISB,1
	decf 0x00,1,0
	bnz loop
    exit:  
	movwf TRISA
	nop
    end


