#include <xc.h>
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)



// using namespace std;

char mystring[20];
int lenStr = 0;

char str[10];

void SYSTEM_Initialize(void);
void OSCILLATOR_Initialize(void);
void UART_Initialize(void);
char * GetString();
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer();
void MyusartRead();
void INTERRUPT_Initialize(void);
void CCP1_Initialize();
void ADC_Initialize() ;
int ADC_Read(int channel);
volatile int value = 18;
void Mode1() // print "Hello world"
{
    ClearBuffer();
    // TODO 
    // Output the result on Command-line Interface.
    UART_Write_Text(" ");

    ClearBuffer();
}

void Mode2() { // Output Voltage 
    ClearBuffer();
    UART_Write_Text("sp2");
    while(1){
       if(strstr(GetString(), "mode1") != NULL){
            break;
        } 
    }
    // breathing led
    
    
    /*int digital = 0;
    float voltage = 0.0;
    while(1) // TODO design a condition. Return to main function when the while loop is over.
    {
        if(strstr(GetString(), "e") != NULL){
            break;
        }
        digital=ADC_Read(0);
        voltage = digital* ((float)5/1023); // 5v / 2^10-1  (10bits)
        // TODO
        // Output the voltage on CLI.
        memset(str, 0, 10);
        sprintf(str, "%.2f", voltage);
        UART_Write_Text(str);
        UART_Write_Text(" ");
        // The voltage must have the second digit after the decimal point.
        for(int i = 0 ; i < 10000 ; i++) ; // a delay time
    }*/
    
    ClearBuffer();
}
void Mode3(){
    ClearBuffer();
    UART_Write_Text("sp3");
    while(1){
       if(strstr(GetString(), "mode1") != NULL){
            break;
        } 
    }
    ClearBuffer();
}
void main(void) 
{
    
    SYSTEM_Initialize() ;
    while(1) {
        // TODO 
        // "clear" > clear UART Buffer()
        if(strstr(GetString(), "clear") != NULL)
            ClearBuffer();
        //else if(strstr(GetString(), "mode1") != NULL)
          //  Mode1();
        // "mode1" > start Mode1 function as above
        else if(strstr(GetString(), "mode2") != NULL)
            Mode2();
        else if(strstr(GetString(), "mode3") != NULL)
            Mode3();
        // "mode2" > start Mode2 function as above*/
    }
    return;
    
}
void SYSTEM_Initialize(void)
{
    // PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize(); //default 1Mhz
    //    TMR2_Initialize();
    //    TMR1_Initialize();
    //    TMR0_Initialize();
    INTERRUPT_Initialize();
    UART_Initialize();
    //CCP1_Initialize();
    ADC_Initialize();
}
void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //disable low priority interrupt
}
void OSCILLATOR_Initialize(void)
{
    // TODO
    // setting OSCILLATOR  frequency
    // 8 MHZ
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    
    
    // RCON = 0x0000;
}

void CCP1_Initialize() {
    //TRISCbits.TRISC2=0;	// RC2 pin is output.
    //CCP1CON=9;		// Compare mode, initialize CCP1 pin high, clear output on compare match
    
    //PIR1bits.CCP1IF=0;
    //IPR1bits.CCP1IP = 1;
    CCPR1L = 18;
    CCPR1H = 18;
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
}
void ADC_Initialize(void) {
    TRISA = 0xff;		// Set as input port
    ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin 
    // ADCON2 = 0x92;  	
    ADFM = 1 ;          // Right Justifie
    ADCON2bits.ADCS = 7; // 
    ADRESH=0;  			// Flush ADC output Register
    ADRESL=0;  
}
void UART_Initialize() {
    
    // TODO
    /*portC pin6/7 is mult with usart setting their tris*/    
    
    TRISCbits.TRISC6 = 0;       // TX for output
    TRISCbits.TRISC7 = 1;       // RX for input
    
    
    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */
    
    // TODO
    // choose the async mode
    
    TXSTAbits.SYNC = 0;    
    
    
    // TODO
    // Setting baud rate
    // using the Baud rate table in data sheet
    
    BAUDCONbits.BRG16 = 0;       
    TXSTAbits.BRGH = 0;
    SPBRG = 12;
    
    
    // TODO
    // Serial enable
    
    RCSTAbits.SPEN = 1;              //open serial port
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;         //Enable Tx
    RCSTAbits.CREN = 1;        //Enable Rx
    
    
    // setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //disable Tx interrupt
    IPR1bits.TXIP = 0;              //Setting Tx as low priority interrupt
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 0;              //Setting Rc as low priority interrupt
}




void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}

char *GetString(){ // 
    return mystring;
}

void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void MyusartRead()
{
    mystring[lenStr] = RCREG;
    UART_Write(mystring[lenStr]);
    lenStr++;
    lenStr %= 10;
//    if(RCREG == '^') TRISC6 = 1;
//    if(RCREG == '$') TRISC6 = 0;
    return ;
}


// old version: 
// void interrupt low_priority Lo_ISR(void)




int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.CHS =  0x00; // Select Channe7
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}






// old version: 
// void interrupt high_priority Hi_ISR(void)
void __interrupt(high_priority) Hi_ISR(void)
{
    if(PIR1bits.CCP1IF == 1) {
        RC2 ^= 1;
        PIR1bits.CCP1IF = 0;
        TMR3 = 0;
    }
    
    return ;
}

void __interrupt(low_priority)  Lo_ISR(void)
{
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
    }
    
   // process other interrupt sources here, if required
    return;
}