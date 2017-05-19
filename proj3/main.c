//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "multi.h"

void main(void)
{

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    // DCO = 3 MHz, SMCLK and MCLK = DCO
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_1;   // DCO = 3 MHz
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;


    __enable_irq();

    P3->DIR |= BIT2;

    Setup_UART();
    Setup_Multi();
    Setup_ADC(33,0);
    Timer_Setup();
    __enable_irq();
    int i = 0;
    Draw_Background();
    while (1) {
        //ADC_RequestNextSample();
        if (i%4 == 0) {
            Refresh_Display();
        }
        i++;
        if (i%1000 == 0)  {
            Draw_Background();
        }
    }
}
