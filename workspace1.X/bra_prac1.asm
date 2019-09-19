LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00

 n equ D'50'
 a equ 1
 sum_hi_addr set 0x01
 sum_lo_addr set 0x00
 index_addr set 0x02
 org 0x00
 
 start:
    clrf sum_hi_addr, a
    clrf sum_lo_addr, a
    clrf index_addr, a
    
    nop
 loop:
    movlw n ; move n(50) to WREG
    cpfsgt index_addr, a; if index>n(W) then skip next line
    bra cal
    bra exit
 cal:
    movf index_addr, 0, a ; move current iterator i to wreg
    addwf sum_lo_addr, 1, a
    movlw 0 ; clear WREG
    addwfc sum_hi_addr, 1, a
    incf index_addr, 1, a
    bra loop; remember move n(after update) to W for loop compare
 exit:
    nop
end


