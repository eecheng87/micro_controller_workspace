LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
    start:
    org 0x1000
    
    array    db    0x01,0x02,0x03,0x04,0x05,0x06,0x07 
    ;array db 'a','b','c' ; store string 'a','b','c' into array at 0x1000 in 
			 ; program mem
			 
    org 0x1100
    movlw upper array
    movwf TBLPTRU,0
    movlw high array
    movwf TBLPTRH,0
    movlw low array
    movwf TBLPTRL,0
    tblrd*+		 ; check out TABLAT = 0x01
    tblrd*+
    tblrd*+
    
   
    
    nop
    end


