//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"

void main(void)
{

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    // DCO = 6 MHz, SMCLK and MCLK = DCO
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_1;   // DCO = 6 MHz
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;


    __enable_irq();

    P3->DIR |= BIT2;

    Setup_UART();
    Timer_Setup();
    Setup_ADC(33,0);

    char arr[8];
    int i;
    while (1) {
        P3->OUT |= BIT2;
        //ADC_RequestNextSample();
        P3->OUT &= ~BIT2;
        while (!ADC_CheckReady()) {}
        ADC_GetFormatedValue(arr);
        for (i = 0; i < 6; i++) {
            UART0Tx(arr[i]);
        }
        UART0Tx(' ');
        UART0Tx('V');
        UART0Tx(0x1b);
        UART0Tx('[');
        UART0Tx('H');
        //for (i = 0; i < 20000; i++) {}
    }
}
