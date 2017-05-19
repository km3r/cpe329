//*****************************************************************************
//
// MSP432 main.c - Project 2
//
//****************************************************************************

#include "msp.h"
#include "proj2.h"
#include <stdlib.h>

void Drive_DAC(unsigned int level);

volatile unsigned int TempDAC_Value = 0;

int *waveform;
int freq = FREQ_100_Hz; // in 100Hz
int fArr[6] = {470/2, 470/4, 470/6, 470/8, 470/10, 4700}; //CCRO values
int pulseMode;
int noise[256];


void main(void)
    {
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer


    P9->DIR = 0xFF;
    P9->OUT = 0x00;


    /* Step 1: Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

    /* Step 2: Configure Flash wait-state to 1 for both banks 0 & 1 */
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL  = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) |
            FLCTL_BANK1_RDCTL_WAIT_1;

    /* Step 3: Configure DCO to 48MHz, ensure MCLK uses DCO as source*/
    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
    /* Select MCLK = DCO, no divider */
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) |
            CS_CTL1_SELM_3;
    CS->KEY = 0;                            // Lock CS module from unintended accesses




    /**
    // DCO = 24 MHz, SMCLK and MCLK = DCO
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_4;   // DCO = 24 MHz
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;
    **/

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

    //setup interupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = 470;

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__UP |
            TIMER_A_CTL_ID__4;

    //SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

    P6->DIR |= BIT0;

    waveform = duty[3];
    freq = FREQ_100_Hz;
    TIMER_A0->CCR[0] = fArr[freq];
    Key key = NONE;
    Key_init();
    int dutyNum = 4;
    while (1) {
        while (key != NONE) key = Get_key();
        while (key == NONE) key = Get_key();
        switch (key) {
            case K1:
                freq--;
                if (freq < FREQ_100_Hz) freq = FREQ_100_Hz;
                if (waveform == noise) freq = FREQ_100_Hz;
                TIMER_A0->CCR[0] = fArr[freq];
                break;
            case K2:
                freq = FREQ_300_Hz;
                if (waveform == noise) freq = FREQ_100_Hz;
                TIMER_A0->CCR[0] = fArr[freq];
                break;
            case K3:
                freq++;
                if (freq > FREQ_500_Hz) {
                    freq = FREQ_500_Hz;
                }
                if (waveform == noise) freq = FREQ_100_Hz;
                TIMER_A0->CCR[0] = fArr[freq];
                break;
            case K4:
                pulseMode = 0;
                break;
            case K5:
                TIMER_A0->CCR[0] = fArr[0];
                waveform = noise;
                break;
            case K6:
                if (waveform != noise)
                    pulseMode = (pulseMode + 1) % 10;
                break;
            case K7:
                dutyNum = 4;
                waveform = duty[4];
                pulseMode = 0;
                break;
            case K8:
                pulseMode = 0;
                waveform = sin;
                break;
            case K9:
                pulseMode = 0;
                waveform = saw;
                break;
            case K_STAR:
                if (waveform != saw && waveform != sin && waveform != noise)
                {
                    dutyNum = dutyNum + 1;
                    if (dutyNum > 8) dutyNum = 8;
                    waveform = duty[dutyNum];
                }
                break;
            case K0:
                if (waveform != saw && waveform != sin && waveform != noise)
                {
                    dutyNum = 4;
                    waveform = duty[4];
                }
                break;
            case K_POUND:
                if (waveform != saw && waveform != sin)
                {
                    dutyNum = dutyNum - 1;
                    if (dutyNum < 0) dutyNum = 0;
                    waveform = duty[dutyNum];
                }
                break;
         }


    }
}

void TA0_0_IRQHandler(void) {
    static char step = 0;
    static int pulseWait = 0;
    static int rand = 35;
    //P6->OUT |= BIT0;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    if (waveform != noise) {
        Drive_DAC(waveform[step]);
        if (step == 0 && pulseWait < (256 * pulseMode))  {
                pulseWait++;
        } else {
            step = step + 1;
            pulseWait = 0;
        }
    } else {
        //rand ^= rand << 2; //alt PRNG
        //rand ^= rand >> 7;
        //rand ^= rand << 5;
        rand = (4321*rand + 420);
        Drive_DAC(rand % 1919);
    }
    //P6->OUT &= ~BIT0;
}


void Drive_DAC(unsigned int level){
  unsigned int DAC_Word = 0;
  //int i;

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
  //for(i = 200; i > 0; i--);                          // Delay 200 16 MHz SMCLK periods
                                                     //to ensure TX is complete by SIMO
  P4->OUT |= BIT1;                                   // Set P4.1   (drive /CS high on DAC)
  return;
}

void delay(){}

void delayMs(int n) {
    int i, j;
    for (j = 0; j < n; j++)
        for (i = 750; i > 0; i--);      /* Delay */
}

Key Get_key(void) {
    Key retMe = NONE;

    P2->OUT = 0xF7;

    //row 1
    P2->DIR = 0x00;
    P2->DIR |= 0xF0;
    P2->OUT &= ~0x10;
    delay();
    unsigned char in = P2->IN & 0x07;
    P2->OUT |=0x10;
    if (in == 0x6)
        retMe = K3;
    if (in == 0x5)
        retMe = K2;
    if (in == 0x3)
        retMe = K1;

    if (retMe != NONE) return retMe;
    // row 2


    P2->OUT &= ~0x20;
    delay();
    in = P2->IN & 0x07;
    P2->OUT |=0x20;
    if (in == 0x6)
        retMe = K6;
    if (in == 0x5)
        retMe = K5;
    if (in == 0x3)
        retMe = K4;

    //row 3

    P5->DIR = 0x00;
    P5->DIR |= 0x10;
    P5->OUT &= ~0x10;
    delay();
    in = P2->IN & 0x07;
    P5->OUT |=0x10;
    if (in == 0x6)
        retMe = K9;
    if (in == 0x5)
        retMe = K8;
    if (in == 0x3)
        retMe = K7;

    //row 4
    P5->DIR = 0x00;
    P5->DIR |= 0x20;
    P5->OUT &= ~0x20;
    delay();
    in = P2->IN & 0x07;
    P5->OUT |=0x20;
    if (in == 0x6)
        retMe = K_POUND;
    if (in == 0x5)
        retMe = K0;
    if (in == 0x3)
        retMe = K_STAR;


    P2->OUT = 0xF7;
    P2->DIR = 0x00;


    delayMs(1);
    return retMe;
}

/* P2(0:2) is cols / input; P2(4:7) is rows/output */
void Key_init(void) {
    P2->DIR = 0x00;
    P2->REN = 0x70;
    //P6->DIR &= ~BIT5;
    //P6->REN |= BIT5;

}
