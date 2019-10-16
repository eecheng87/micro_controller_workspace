LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
    
   
pushr	macro	arg
	movff	arg,POSTINC1
	endm
popr	macro	arg
	movff 	POSTDEC1,arg
	movff	INDF1,arg
	endm
alloc_stk   macro	n
	movlw	n
	addwf 	FSR1L,F,A
	movlw 	0x00
	addwfc	FSR1H,F,A
	endm
dealloc_stk macro	n
	movlw	n
	subwf	FSR1L,F,A
	movlw	0x00
	subwfb	FSR1H,F,A
	endm

MOVLF macro literal,f
	movlw literal
	movwf f,A
	endm

ADDFFF macro f1,f2,f_res
	movf f1,W
	addwf f2,W,A
	movwf f_res,A
	endm

	
	n equ D'11'
	f1 equ 0x31
	f2 equ 0x32
	f3 equ 0x02
	n_offset equ 0x03
	
start:
    lfsr FSR1,0x000
    MOVLF 0x00,f1 ; initial f1
    MOVLF 0x01,f2 ; f2
    MOVLF n,0x03    ; n
    rcall fib
    goto done
fib: 
    movlw 0x01
    cpfsgt n_offset,A ; skip if f>w 
    return ; done
    MOVLF 0x20,PCL; haven't done
    ADDFFF f1,f2,f3
    movff f2,f1
    movff f3,f2
    decf n_offset,F,A
    MOVLF 0x16,PCL; jump back to fib
done:
    movff f2,0x33
    nop
    end
  


