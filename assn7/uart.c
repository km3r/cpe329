/*
 * uart.c
 *
 *  Created on: May 8, 2017
 *      Author: kmrosent
 */

#include "uart.h"
#include "dac.h"
#include "msp.h"

void Setup_UART() {
    val = 0;
    statusFlag = 0;

    __disable_irq();

    EUSCI_A0->CTLW0 |= BIT0;
    EUSCI_A0->MCTLW = 0;
    EUSCI_A0->CTLW0 = 0x0081;
    EUSCI_A0->BRW = 26;
    P1SEL0 |= (BIT2 + BIT3);
    P1SEL1 &= ~(BIT2 + BIT3);
    EUSCI_A0->CTLW0 &= ~BIT0;
    EUSCI_A0->IFG |= EUSCI_A_IFG_RXIFG;
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    //NVIC_SetPriority(EUSCIA0_IRQn, 4);
    NVIC_EnableIRQ(EUSCIA0_IRQn);
    __enable_irq();
}

int readFlag() {
    return statusFlag;
}

void setFlag() {
    statusFlag = 1;
}
void clearFlag() {
    statusFlag = 0;
}
int readVal() {
    int temp = val;
    val = -1;
    return temp;
}

/* read a character from UART0 */
unsigned char UART0Rx(void) {
    char c;
    while(!(EUSCI_A0->IFG & 0x01)) ;
    c = EUSCI_A0->RXBUF;
    return c;
}

/* write a character to UART */
unsigned char UART0Tx(unsigned char c) {
    while(!(EUSCI_A0->IFG&0x02)) ;
    EUSCI_A0->TXBUF = c;
    return c;
}

void EUSCIA0_IRQHandler(void) {
    char c = EUSCI_A0->RXBUF;
    static int intVal = 0;

    if (c >= '0' && c <= '9') {
        intVal *= 10;
        intVal += c - '0';
    }

    if (c == '\r') {
        while(!(EUSCI_A0->IFG&0x02)) ;
        EUSCI_A0->TXBUF = c;
        c = '\n';
        val = intVal;
        intVal = 0;
        statusFlag = 1;
    }


    while(!(EUSCI_A0->IFG & 0x02)) {}
    EUSCI_A0->TXBUF = c;
}
