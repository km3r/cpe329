/*
 * uart.c
 *
 *  Created on: May 8, 2017
 *      Author: kmrosent
 */

#include "uart.h"
#include "msp.h"
#include "sound.h"

void Setup_UART() {
    val = 0;
    statusFlag = 0;


    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->MCTLW = 0;
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SSEL1 + EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->BRW = 0x09c4;
    P1SEL0 |= (BIT2 + BIT3);
    P1SEL1 &= ~(BIT2 + BIT3);
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->IFG |= EUSCI_A_IFG_RXIFG;
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    //NVIC_SetPriority(EUSCIA0_IRQn, 4);
    NVIC_EnableIRQ(EUSCIA0_IRQn);

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

void UART_String(char * str, int len, int newline) {
    int i;
    for (i = 0; i < len; i++) {
        UART0Tx(str[i]);
    }
    if (newline == 1) {
        UART0Tx('\033');
        UART0Tx('E');
    }
}


void EUSCIA0_IRQHandler(void) {
    static int wave_num = 0;
    char c = EUSCI_A0->RXBUF;
    switch (c) {
        case 'a':
            wave_num = (wave_num + 1);
            break;
        case 'b':
            Update_Period(wave_num, 1);
            break;
        case 'c':
            Update_Period(wave_num, -1);
            break;
        case 'd':
            Update_Freq(wave_num, 1);
            break;
        case 'e':
            Update_Freq(wave_num, 1);
            break;
    }
    if (c == '\r') {
        UART0Tx(c);
        c = '\n';
    }

    while(!(EUSCI_A0->IFG & 0x02)) {}
    EUSCI_A0->TXBUF = c;
}
