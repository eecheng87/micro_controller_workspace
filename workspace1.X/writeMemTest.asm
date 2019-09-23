LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    start:
	movlw 0x1c
	movwf 0x101,0; store in access bank
	movlw 0x1b
	movlb 0x1
	movwf 0x101,0; store in 001 cause overiding BSR
	movwf 0x101,1
	movlw 0x0c
	movlb 0x01
	movwf 0x002,1
	movlw 0x0a
	movwf 0x003,0
	movwf 0x004
	nop
    
    

    end


