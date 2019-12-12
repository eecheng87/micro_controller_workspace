/*
 * File:   newmain.c
 * Author: steven
 *
 * Created on December 5, 2019, 6:38 PM
 */

#include <xc.h>
#pragma config OSC  = INTIO67,WDT=OFF,LVP=OFF
#pragma PBADEN = 1 //set AN0~AN12 as analog input

void __interrupt(high_priority)Hi_ISR(void){
    // AD conversion finish
    if(PIR1bits.ADIF){
        
    }else if(PIR2bits.CCP2IF){
        
    }
    return;
}
void main(void) {
    return;
}
