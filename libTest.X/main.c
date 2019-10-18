/*
 * File:   main.c
 * Author: steven
 *
 * Created on October 16, 2019, 11:54 PM
 */


#include <xc.h>
#include "lib1.h"
void main(void) {
    unsigned int a = 9;
    unsigned int b = 3;
    volatile int sum = add(a,b);
    volatile int diff = sub(a,b);
    while(1);
    return;
}
