//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "uart.h"
#include "dac.h"
#include "sound.h"
#include "math.h"



void main(void){
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    // 24 mhz
    CS->KEY = CS_KEY_VAL; // unlock CS registers
    CS->CTL0 = 0; // clear register CTL0
    CS->CTL0 = CS_CTL0_DCORSEL_4;
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3; // select clock sources
    CS->KEY = 0; // lock the CS registers
	
    //Setup_UART();
    Setup_DAC();

    __enable_irq();
    Sound_Init(0, 21, 3, 18);

    Sound_Go();
}
