LIST p=18f4520
    #include "p18f4520.inc"

; CONFIG1H
  CONFIG  OSC = INTIO67         ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOREN = SBORDIS       ; Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
  CONFIG  BORV = 3              ; Brown Out Reset Voltage bits (Minimum setting)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 32768         ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
  CONFIG  CCP2MX = PORTC        ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
  CONFIG  PBADEN = ON           ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = ON            ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)
  
  org 0x00
  L1 set 0x03
  L2 set 0x04
 
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
    
  goto Initial
  
  ISR:
    org 0x08
    
     
    ;clrf LATD 
    ;DELAY d'180',d'60'
    ;DELAY d'180',d'60'
    ;DELAY d'180',d'60'
    btg 0x00,0,0 ; toggle 0x00 bit 0
    bcf INTCON,INT0IF ; clear external interrupt flag
    ;bsf PORTD,1,0
    ;Delay
    ;movlw count_val
    ;movwf tmr0l,0
    ;bcf intcon,TMR0IF // disable TMR0 Overflow Interrupt Flag bit 
    ;bcf portd,1,0
    retfie
  Initial:
    ;clrf porta
    ;clrf lata
    ;bsf trisa,4
    clrf TRISD
    clrf PORTD
    clrf TRISB
    clrf PORTB
    clrf LATB
    bsf TRISB,0,0
    clrf 0x00
    clrf 0x01
    bsf 0x00,0
    bsf 0x01,0 ; initial led position at rd0
    
    movlw 0x0f
    movwf ADCON1   
    ;movlw b'11111000'
    ;movwf t0con,0
    
    bsf RCON,IPEN ; allow global interrupt
    bsf INTCON,GIE ; enable all unmasked interrupt
    bsf INTCON,INT0IE ; Enables the external interrupt
    bsf INTCON2,INTEDG0
    
   Main:
    ; 0x00 deciding lighten direction
    ; 0x01 deciding lighten position
    movff 0x01,LATD
    DELAY d'180',d'60'
    clrf LATD 
    
    btfsc 0x00,0,0
    goto light_right
    goto light_left
   light_right:
    btfss 0x01,0,0 ; skip if bit 0 is 1
    goto gene_r
    goto except_r
    
    gene_r:
    rrncf 0x01,F,0
    goto Main
    except_r:
    clrf 0x01
    bsf 0x01,3,0
    goto Main
    
   light_left:
    btfss 0x01,3,0 ; skip if bit 3 is 1
    goto gene_l
    goto except_l
    
    gene_l:
    rlncf 0x01,F,0
    goto Main
    except_l:
    clrf 0x01
    bsf 0x01,0,0
    goto Main
    
    
    end

