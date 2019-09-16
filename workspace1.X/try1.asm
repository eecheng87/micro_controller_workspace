LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
initial:
    ; reg 0x0 is for counting iterate time
    ; reg 0x1 is for number each iterate added
    ; reg 0x2 is temp answer
    clrf 0x0
    clrf 0x1
    clrf 0x2
start:
    movlw 0x8
    movwf 0x1,0
loop:
    btfss 0x0,3,0
    goto label
    goto exit
label:
    incf 0x0,1,0
    movf 0x2,0,0; move 0x2 to WREG
    addwf 0x1,0,0; add 0x1 to WREG
    movwf 0x2,0; move WREG to 0x3
    incf 0x1,1,0; increase 0x1
    goto loop
exit:
    movf 0x2,0,0; move 0x2 to WREG
    nop
end

