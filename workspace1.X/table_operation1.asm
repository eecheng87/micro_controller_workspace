LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
   start:
	clrf TBLPTRU,0
	clrf TBLPTRH,0 ; cuz is in SFR(access bank)
	movlw 0x00
	movwf TBLPTRL,0
	tblrd*
	nop
end


