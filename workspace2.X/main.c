/*
 * File:   main.c
 * Author: steven
 *
 * Created on October 16, 2019, 11:18 PM
 */


#include <xc.h>
extern unsigned int add(unsigned int a, unsigned int b);
void main(void) {
    volatile unsigned int result = add(12,10);
    while(1);
    return;
}
