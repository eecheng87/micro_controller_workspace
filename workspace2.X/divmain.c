/*
 * File:   divmain.c
 * Author: steven
 *
 * Created on October 17, 2019, 11:04 PM
 */


#include <xc.h>
extern unsigned int divide(unsigned int dd, unsigned int dr);
void main(void) {
    volatile unsigned int res = divide(255,13);
    //volatile unsigned char quo = ;
    //volatile unsigned char rem = ;
    while(1);
    return;
}
