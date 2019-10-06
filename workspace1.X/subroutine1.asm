LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF

    	
	radix dec

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

acnt	equ D'32'
lp_cnt	equ 3
ar_max	equ 4
ar_cnt	equ -3
ar_xup	equ -4
ar_xhi	equ -5
ar_xlo 	equ -6

array_max set 0x00
	

	org 0x00
	goto start
	org 0x08
	retfie
	org 0x18
	retfie
start:

	lfsr 	FSR1,0x100
	movlw 	low arr_x
	pushr	WREG
	movlw 	high arr_x
	pushr 	WREG
	movlw 	upper arr_x
	pushr 	WREG
	movlw acnt
	pushr WREG
	
	call 	find_m,FAST
	movff	PRODL,array_max
	dealloc_stk	4
	nop


find_m:
	pushr	FSR2L
	pushr	FSR2H
	movff 	FSR1L,FSR2L
	movff 	FSR1H,FSR2H
	pushr 	TBLPTRL
	pushr	TBLPTRH
	pushr	TBLPTRU
	alloc_stk	2

	movlw 	ar_cnt
	movff	PLUSW2,PRODL
	decf	PRODL,F,A
	movlw	lp_cnt
	movff	PRODL,PLUSW2
	movlw	ar_xlo
	movff	PLUSW2,TBLPTRL
	movlw 	ar_xhi
	movff	PLUSW2,TBLPTRH
	movlw	ar_xup
	movff	PLUSW2,TBLPTRU
	tblrd*+
	
	movlw	ar_max
	movff	TABLAT,PLUSW2
	
cmp_lp:
	movlw 	lp_cnt
	tstfsz	PLUSW2
	goto	next
	goto	done
next:
	movlw	lp_cnt
	decf	PLUSW2,F
	tblrd*+
	movlw	ar_max
	movf	PLUSW2,W,A
	cpfsgt	TABLAT ; compare f with W, skip if f>w
	goto	cmp_lp
	movlw	ar_max ; change max
	movff	TABLAT,PLUSW2
	goto	cmp_lp
done:
	movlw 	ar_max
	movff	PLUSW2,PRODL
	dealloc_stk	2
	popr	TBLPTRU
	popr	TBLPTRH
	popr	TBLPTRL
	popr	FSR2H
	popr	FSR2L
	return	FAST
	
arr_x	db 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
	db 17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32


end


/*
	memory layout
	
	empty <---- fsr1
	ar_max
	lp_cnt
	TBLPTRU      
	H
	L      <---- fsr2
	FSR2H
	FSR2L
	ar_cnt
	arr_xup      } here is data passing as parameter ( push before caller and using in subroutine)
	xhi
	xlo
	
	
	
	*/