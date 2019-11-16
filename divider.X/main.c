/*
 * File:   main.c
 * Author: steven
 *
 * Created on October 18, 2019, 12:48 PM
 */

#include <xc.h>
//extern unsigned int divide(unsigned int dd, unsigned int dr);
extern unsigned int add(unsigned int a,unsigned int b);
void main(void) {
    volatile unsigned int res = divide(254,12);
    //volatile unsigned int result = add(12,8);
    volatile unsigned char quo = *(char*)&res;
    volatile unsigned char rem = *((char*)&res+1);
    while(1);
    return;
}
