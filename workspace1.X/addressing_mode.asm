LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
      
write MACRO Var, Address
	movlw Var
	movwf Address, 1
	ENDM 	
    org 0x00

		


start:
    ;movlb 2
    movlw 14
    movwf 10, 1
	write 0x39, 100
	write 0x4e, 200
	nop
    
    end


