/*
 * timer.c
 *
 *  Created on: May 15, 2017
 *      Author: kmrosent
 */

#include "timer.h"
#include "sound.h"
#include "msp.h"

void Timer_Setup() {
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = 1500;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__UP |
            TIMER_A_CTL_ID__1;

    NVIC_EnableIRQ(TA0_0_IRQn);
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    Sound_Go();
}
