//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "dac.h"
#include "uart.h"

void main(void) {
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    /**
    // DCO = 24 MHz, SMCLK and MCLK = DCO
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_4;   // DCO = 24 MHz
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;
    **/

    Setup_DAC();
    Setup_UART();




    while (1) {
        if (readFlag()) {
            clearFlag();
            int val = readVal();
            if (val <= 4095 && val >= 0) {
                Drive_DAC(val);
            }
        }
    }
}
