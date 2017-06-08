/*
 * dac.c
 *
 *  Created on: May 8, 2017
 *      Author: kmrosent
 */

#include "dac.h"
#include "msp.h"

void Setup_DAC(void) {
    // Configure port bits for SPI
    P4->DIR |= BIT1;                     // Will use BIT4 to activate /CE on the DAC
    P1SEL0 |= BIT6 + BIT5;               // Configure P1.6 and P1.5 for UCB0SIMO and UCB0CLK
    P1SEL1 &= ~(BIT6 + BIT5);            //

    // SPI Setup
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;   // Put eUSCI state machine in reset

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |   // Remain eUSCI state machine in reset
                      EUSCI_B_CTLW0_MST   |   // Set as SPI master
                      EUSCI_B_CTLW0_SYNC  |   // Set as synchronous mode
                      EUSCI_B_CTLW0_CKPL  |   // Set clock polarity high
                      EUSCI_B_CTLW0_MSB;      // MSB first

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK; // SMCLK
    EUSCI_B0->BRW = 0x01;                         // divide by 16, clock = fBRCLK/(UCBRx)
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;      // Initialize USCI state machine, SPI
                                                  // now waiting for something to
                                                  // be placed in TXBUF

    EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;  // Clear TXIFG flag
}

void Drive_DAC(unsigned int level){
    unsigned int DAC_Word = 0;
    int i;

    DAC_Word = (0x1000) | (level & 0x0FFF);   // 0x1000 sets DAC for Write
                                              // to DAC, Gain = 2, /SHDN = 1
                                              // and put 12-bit level value
                                              // in low 12 bits.
    P4->OUT &= ~BIT1;                                   // Clear P4.1 (drive /CS low on DAC)
                                                        // Using a port output to do this for now
    EUSCI_B0->TXBUF = (unsigned char) (DAC_Word >> 8);  // Shift upper byte of DAC_Word
                                                        // 8-bits to right
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));              // USCI_A0 TX buffer ready?
    EUSCI_B0->TXBUF = (unsigned char) (DAC_Word & 0x00FF);     // Transmit lower byte to DAC
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));      // Poll the TX flag to wait for completion
    for(i = 200; i > 0; i--);                          // Delay 200 16 MHz SMCLK periods
                                                       //to ensure TX is complete by SIMO
    P4->OUT |= BIT1;                                   // Set P4.1   (drive /CS high on DAC)
    return;
}
