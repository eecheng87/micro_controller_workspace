LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
      
write MACRO Var, Address
	movlw Var
	movwf Address,1
	ENDM 	
    org 0x00
    offset equ 0x13
		
 

start:
	movlb 1
	write 0x39, 0x0100
	write 0x4e, 0x0101
	write 0x4f, 0x0102
	write 0x1f, 0x0103
	write 0x0d, 0x0104
	write 0x56, 0x0105
	write 0x60, 0x0106
	write 0x0d, 0x0107
	write 0x52, 0x0108
	write 0x4e, 0x0109
	write 0x60, 0x010a
 	write 0x66, 0x010b

 
	lfsr FSR1, 0x0100
	lfsr FSR2, 0x0200

loop:
	movlw 0x0b
	cpfsgt FSR1L, 0; should specify a or ignore, cuz FSR is in SFR(access b)  
	goto cal
	goto exit
cal:
    	movf POSTINC1, 0 
	addlw offset
	movlb 2
	movwf POSTINC2, 0
	goto loop	
exit:
	nop
    
    end


