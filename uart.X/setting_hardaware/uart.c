#include <xc.h>
    //setting TX/RX

char mystring[20];
int lenStr = 0;

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