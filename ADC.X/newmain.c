/*
 * File:   newmain.c
 * Author: steven
 *
 * Created on December 5, 2019, 7:11 PM
 */

#include <xc.h>
#pragma config OSC  = INTIO67,WDT=OFF,LVP=OFF
#pragma PBADEN = 1 //set AN0~AN12 as analog input

int index = 0;
volatile int result[10] = {0};
void __interrupt(high_priority)Hi_ISR(void){
    // AD conversion finish
    if(PIR1bits.ADIF){
        // ad conversion finish
        // store result
        result[index] = ADRES;
        PIR1bits.ADIF = 0;
        if(ADRES<128){
            PORTD = 0;
        }else if(ADRES<256){
            PORTD = 0;
            PORTDbits.RD0 = 1;
        }else if(ADRES<384){
            PORTD = 0;
            PORTDbits.RD1 = 1;
        }else if(ADRES<512){
            PORTD = 0;
            //PORTD = 3;
            PORTDbits.RD0 = 1;
            PORTDbits.RD1 = 1;
        }else if(ADRES<640){
            PORTD = 0;
            PORTD = 4;
        }else if(ADRES<768){
            PORTD = 0;
            PORTD = 5;
        }else if(ADRES<896){
            PORTD = 0;
            PORTD = 6;
        }else{
            PORTD = 0;
            PORTD = 7;
        }
    }else if(PIR2bits.CCP2IF){
        // delay 0.05s 
        TMR3 = 0;
        PIR2bits.CCP2IF = 0;
        if(index==9){
            index = 0;
        }else{
            index++;
        }
    }
    return;
}
void adc_init(){
    // Voltage Reference Configuration bit
    // set 2!!
    ADCON1bits.VCFG = 0;
    ADCON1bits.PCFG = 0;
    // adc input : AN0
    ADCON0bits.CHS = 0;
    // set input (this line is super important)
    TRISAbits.RA0 = 1;
    // A/D Converter module is enabled 
    ADCON0bits.ADON = 1;
    // Right justified 
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 0b101;
    ADCON2bits.ADCS = 0b100;
    // configure interrupt
    PIE1bits.ADIE = 1;// Enables the A/D interrupt
    PIR1bits.ADIF = 0;//The A/D conversion is not complete 
    IPR1bits.ADIP = 1;//A/D Converter Interrupt Priority bit
    RCONbits.IPEN = 1; //allow global interrupt
    INTCONbits.GIE = 1; //enable all unmasked interrupt
    
    
}
void ccp2_init(){
    CCP2CONbits.CCP2M = 0b1011;
    // configure ccp2 interrupt
    PIR2bits.CCP2IF = 0;
    PIE2bits.CCP2IE = 1;
    IPR2bits.CCP2IP = 1; // high pri
    CCPR2 = 50000; // 0.05s
    
}
void timer3_init(){
    T3CONbits.RD16 = 1;
    T3CONbits.T3CCP1 = 1;
    T3CONbits.T3CCP2 = 0;
    T3CONbits.T3CKPS = 0;
    T3CONbits.TMR3CS = 0; // internel clock
    T3CONbits.TMR3ON = 1;
    //TMR3 = 0;
}
void main(void) {
    TRISD = 0;
    adc_init();
    ccp2_init();
    timer3_init();
    while(1);
    return;
}
