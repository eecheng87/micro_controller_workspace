LIST p=18f4520
#include<p18f4520.inc>
  ;CONFIG  OSC = INTIO67         ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
  ;CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  ;CONFIG  PBADEN = OFF           ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
  ;CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  ;CONFIG  FOSC = INTOSC
  ;CONFIG IESO = ON
  org 0x00
  movlw 0x10
  movwf 0x00
  movlw 0x20
  movff 0x00,WREG
  nop
  end


