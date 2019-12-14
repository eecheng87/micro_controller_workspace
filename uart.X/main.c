#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;

char str[10];

void Mode1() // print "Hello world"
{
    ClearBuffer();
    // TODO 
    // Output the result on Command-line Interface.
    UART_Write_Text("Hello ");
    UART_Write_Text("World!");

    ClearBuffer();
}

void Mode2() { // Output Voltage 
    ClearBuffer();
    int digital = 0;
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
        else if(strstr(GetString(), "mode1") != NULL)
            Mode1();
        // "mode1" > start Mode1 function as above
        else if(strstr(GetString(), "mode2") != NULL)
            Mode2();
        // "mode2" > start Mode2 function as above*/
    }
    return;
    
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