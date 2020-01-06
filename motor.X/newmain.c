 
#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP =OFF
#define _XTAL_FREQ 500000
#include <xc.h>
volatile int value = 1024;
int index = 0;
int offset = 10;
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
    //RCONbits.IPEN = 1; //allow global interrupt
    //INTCONbits.GIE = 1; //enable all unmasked interrupt
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
void __interrupt(high_priority) isr(void){
    if(PIR1bits.ADIF){
        // ad conversion finish
        // store result
        PIR1bits.ADIF = 0;
        if(ADRES<250){
            offset = 1;
        }else if(ADRES<502){
            offset = 2;
        }else if(ADRES<750){
            offset = 6;
        }else{
            offset = 10;
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
   
}
void main(void) {
    adc_init();
    ccp2_init();
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
    PR2 = 255;
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
    while(1){
        while(value != 1){
            CCPR1L = value>>2;
            CCP1CONbits.CCP1X = value%2;
            CCP1CONbits.CCP1Y = (value%4)>>1;
            --value;
            LATDbits.LATD3 = 1;
            for(int g=0; g<offset;g++);
            LATDbits.LATD3 = 0;
        }
        //value = 18;
        while(value != 1024){
            ++value; 
            CCPR1L = value>>2;
            CCP1CONbits.CCP1X = value%2;
            CCP1CONbits.CCP1Y = (value%4)>>1;
            for(int g=0; g<offset;g++);
        }   
    };
    return;
}

