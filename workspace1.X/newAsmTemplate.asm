LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
  movlw 0x10
  movwf 0x00
  movlw 0x20
  movff 0x00,WREG
  nop
  end


