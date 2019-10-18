#include "xc.inc"
    
GLOBAL _divide
PSECT mytext, local, class=CODE,reloc=2
    count set 0x00
    rem set 0x11
    quo set 0x12
    dsr set 0x13
    dvd set 0x14
 _divide:
    ; initial data
    movff 0x01, quo ; move dd to dvd
    movff 0x03, dsr ; move dr to dsr
    clrf rem,0
    movlw 0x08
    movwf count,0
    
    loop:
    bcf STATUS,C,0
    rlcf quo,F,0
    rlcf rem,F,0
    movf dsr,W
    subwf rem,W,0 ; store rem-divisor's result in WREG
    btfss STATUS,C,0 ; skip if set -> C=0 means negative
    goto nega
    bsf quo,0,0; positive
    movwf rem,0
    goto next
    nega:
    bcf quo,0,0
    next:
    decfsz count,F,0 
    goto loop
    goto done
    done:
    movff rem,0x03
    movff quo,0x01
    return
    
    
    


