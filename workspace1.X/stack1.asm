LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
   
    pushr macro arg
	movff arg,POSTINC1
	endm
	
    popr macro arg
	movff POSTDEC1,arg ; current pointer is at top(no value), so should
	movff INDF1,arg	   ; decrease for accessing 'real' top data
	endm		   ; movff POSTDEC1,arg : 'arg' is meaningless

    push_dat macro arg
	movlw arg
	pushr WREG
	endm
	
   start:
    lfsr  FSR1,0x100
    movlw 0x12
    pushr WREG
    movlw 0x11
    pushr WREG
    popr 0x00
    popr 0x01

    nop
    
end


