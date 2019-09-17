LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    org 0x00
; demo for how to use #define as variable   
#define REG1 0x3
    
clrf WREG
movlw 0x8
movwf REG1,0
nop

; practice some operation between different BSR bank
movlb 0x2 ; switch BSR to 2
movwf 0x1,1
addlw 0x2 ; modify WREG value
movlb 0x1 ; switch BSR to 1
movwf 0x1,1
addlw 0x1 ; modify WREG again
nop
    
movf 0x1,0x0,0x1
nop ; expected WREG value is 0xa
movlb 0x2
movf 0x1,0x0,0x1
nop ; expected WREG value is 0x8
    
end



