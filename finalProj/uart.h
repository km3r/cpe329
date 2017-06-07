/*
 * uart.h
 *
 *  Created on: May 8, 2017
 *      Author: kmrosent
 */

#ifndef UART_H_
#define UART_H_

int statusFlag;
int val;
void Setup_UART();
int readFlag();
void setFlag();
void clearFlag();
int readVal();
unsigned char UART0Rx(void);
unsigned char UART0Tx(unsigned char c);
void UART_String(char * str, int len, int newline);
void EUSCIA0_IRQHandler(void);

#endif /* UART_H_ */
