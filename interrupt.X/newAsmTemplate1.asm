LIST p=18f4520
#include<p18f4520.inc>
  CONFIG  OSC = INTIO67         ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  PBADEN = OFF           ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  ;CONFIG  FOSC = INTOSC
  ;CONFIG IESO = ON
  org 0x00
  L1 set 0x01
  L2 set 0x02

  DELAY MACRO num1,num2
    local LOOP1
    local LOOP2
    movlw num2
    movwf L2
    LOOP2:
	movlw num1
	movwf L1
    LOOP1:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	decfsz L1,1
	goto LOOP1
	decfsz L2,1
	goto LOOP2
    endm
  detect_btn MACRO 
    clrf LATD
    clrf WREG
    btfsc PORTA,4    
    bsf WREG,0
    btfsc WREG,0
    goto before_start
    endm
   detect_btn_aft MACRO 
    clrf LATD
    clrf WREG
    btfsc PORTA,4    
    bsf WREG,0
    btfsc WREG,0
    goto start
    endm
   init_led MACRO
    clrf 0x10
    bsf 0x10,5
    bsf 0x10,6
    bsf 0x10,7
    bsf 0x10,0
    endm
   lighten MACRO
    movff 0x10,LATD
    DELAY d'180',d'60'
    clrf LATD
    rlncf 0x10
    endm
    init:
    ;bsf CONFIG1H,7
    ;bsf OSCTUNE,6
    ;clrf OSCCON
    ;bsf OSCCON,6
    ;bsf OSCCON,6
    CLRF    TRISD	; set PORTD as output
    CLRF    LATD	; 0000 0000 , RD0~RD7 = 0
    CLRF    TRISA
    clrf PORTA
    bsf TRISA,4
    init_led
 
    before_start:
	detect_btn
    start:
    btfsc 0x10,4 ; skip if clear
    init_led
    detect_btn_aft
    lighten
    goto start
    
    end


