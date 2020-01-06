 
#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP =OFF
#define _XTAL_FREQ 500000
#include <xc.h>
volatile int value = 18;
int count = 2;
int start = 0;
void __interrupt(high_priority) isr(void){
    if(INT0IF&&INT0IE){
        /*while(value != 8){
            CCPR1L = value>>2;
            CCP1CONbits.CCP1X = value%2;
            CCP1CONbits.CCP1Y = (value%4)>>1;
            --value;
            LATDbits.LATD3 = 1;
            for(int g=0; g<100;g++);
            LATDbits.LATD3 = 0;
        }
        //value = 18;
        while(value != 76){
            ++value; 
            CCPR1L = value>>2;
            CCP1CONbits.CCP1X = value%2;
            CCP1CONbits.CCP1Y = (value%4)>>1;
            for(int g=0; g<100;g++);
        }
        /*value = 18;
        CCPR1L = value;
        CCPR1H = value;*/
       
        start = 1;
             
       INTCONbits.INT0IF = 0;       
    }else if(TMR3IF&&start==1&&count>=0){
        PIR2bits.TMR3IF = 0;
        LATD = 0xff;
        CCPR1L-=1;
        TMR3 = 3036;
        count--;  
    }

   
}

void timer3_init(){
    T3CONbits.RD16 = 1;
    //T3CONbits.T3CCP1 = 1;
    //T3CONbits.T3CCP2 = 0;
    T3CONbits.T3CKPS = 1;
    T3CONbits.TMR3CS = 0; // internel clock
    T3CONbits.TMR3ON = 1;
    // interrupt configuration
    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 1;
    IPR2bits.TMR3IP = 1;
    TMR3 = 3036;
    //TMR3 = 0;
}
void main(void) {
    timer3_init();
    // allow global interrupt
    RCONbits.IPEN = 1;
    // enable all unmasked interrupt
    INTCONbits.GIE = 1;
    // Enables the external interrupt
    INTCONbits.INT0IE = 1;
    // External Interrupt 0 Edge Select bit 
    INTCON2bits.INTEDG0 = 1;
    //button
    TRISB = 0;
    PORTB = 0;
    LATB = 0;
    TRISBbits.RB0 = 1;
    TRISD = 0;
    PORTD = 0;
    
    TRISDbits.RD3 = 0;
    CCPR1L = 18;
    //CCPR1H = 18;
    //PIE1bits.TMR2IE = 1;
    //PIR1bits.TMR2IF = 1;  
    //(1)Set the PWM period by writing to the PR2 register.
    PR2 = 155;
    //(2)Set the PWM duty cycle by writing to the CCPRxL register
    //and CCPxCON<5:4> bits. 
    CCP1CONbits.DC1B = 3;
    //(3)Make the CCPx pin an output by clearing the appropriate TRIS bit
    TRISCbits.RC2 = 0;
    //(4)Set the TMR2 prescale value
    T2CONbits.T2CKPS = 3; 
    //(4)enable Timer2 by writing to T2CON.
    T2CONbits.TMR2ON = 1;
    //Internal Oscillator Frequency Select bits : 500kHz
    OSCCONbits.IRCF = 3; 
    //(5)Configure the CCPx module for PWM operation. 
    CCP1CONbits.CCP1M = 12;
    while(1);
    return;
}
