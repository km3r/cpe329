//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "adc.h"
#include "uart.h"

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    __enable_irq();

    Setup_UART();
    Setup_ADC(33,0);
	char arr[8];
	int i;
    while (1) {
        UART0Tx('V');
        ADC_RequestNextSample();
        while (!ADC_CheckReady()) {}
        ADC_GetFormatedValue(arr);
        for (i = 0; i < 6; i++) {
            UART0Tx(arr[i]);
        }
        UART0Tx(' ');
        UART0Tx('V');
        UART0Tx('\r');
        UART0Tx('\n');
        for (i = 0; i < 20000; i++) {}
    }
}
