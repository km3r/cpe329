/*
 * key.c
 *
 *  Created on: May 22, 2017
 *      Author: kmrosent
 */

#include "key.h"
#include "msp.h"

void delay() {}

void delayMs(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 750; i > 0; i--);      /* Delay */
}

Key Get_key(void) {
    Key retMe = NONE;

    P2->OUT = 0xF7;

    //row 1
    P2->DIR = 0x00;
    P2->DIR |= 0xF0;
    P2->OUT &= ~0x10;
    delay();
    unsigned char in = P2->IN & 0x07;
    P2->OUT |=0x10;
    if (in == 0x6)
        retMe = K3;
    if (in == 0x5)
        retMe = K2;
    if (in == 0x3)
        retMe = K1;

    if (retMe != NONE) return retMe;
    // row 2


    P2->OUT &= ~0x20;
    delay();
    in = P2->IN & 0x07;
    P2->OUT |=0x20;
    if (in == 0x6)
        retMe = K6;
    if (in == 0x5)
        retMe = K5;
    if (in == 0x3)
        retMe = K4;

    //row 3

    P5->DIR = 0x00;
    P5->DIR |= 0x10;
    P5->OUT &= ~0x10;
    delay();
    in = P2->IN & 0x07;
    P5->OUT |=0x10;
    if (in == 0x6)
        retMe = K9;
    if (in == 0x5)
        retMe = K8;
    if (in == 0x3)
        retMe = K7;

    //row 4
    P5->DIR = 0x00;
    P5->DIR |= 0x20;
    P5->OUT &= ~0x20;
    delay();
    in = P2->IN & 0x07;
    P5->OUT |=0x20;
    if (in == 0x6)
        retMe = K_POUND;
    if (in == 0x5)
        retMe = K0;
    if (in == 0x3)
        retMe = K_STAR;


    P2->OUT = 0xF7;
    P2->DIR = 0x00;


    delayMs(1);
    return retMe;
}

/* P2(0:2) is cols / input; P2(4:7) is rows/output */
void Key_init(void) {
    P2->DIR = 0x00;
    P2->REN = 0x70;
    //P6->DIR &= ~BIT5;
    //P6->REN |= BIT5;

}
