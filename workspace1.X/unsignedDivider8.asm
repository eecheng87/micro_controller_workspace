LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00

	count set 0x00
	rem set 0x01
	quo set 0x02
	dsr set 0x03
	dvd set 0x04
	dd equ 0x13
	dr equ 0x3
	
	start:
	; initial data
	movlw dd
	movwf quo,A
	movlw dr
	movwf dsr,A
	clrf rem,A
	movlw 0x08
	movwf count

	loop:
	bcf STATUS,C,A ; clear carry bit
	rlcf quo,F,A ; if carry is 1, after rotate
	rlcf rem,F,A ; then rem least significant bit will set 1 after rotate
	movf dsr,W,A
	subwf rem,W,A 
	btfss STATUS,C,A ; skip if carry is 1 (positive)
	goto neg
	bsf quo,0,A
	movwf rem,A
	goto next
	neg:
	bcf quo,0,A ; set the quo bit to zero
	next:
	decfsz count,F,A
	goto loop
	
	nop

end
