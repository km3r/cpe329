/*
 * adc.c
 *
 *  Created on: May 10, 2017
 *      Author: kmrosent
 */

#include "adc.h"
#include "msp.h"

void Setup_ADC(int v_h, int v_l) {

    vL = v_l;
    vH = v_h;
    adcflag = F_ADC_NO_OP;

    P5->SEL0 |= BIT5;
    P5->SEL1 |= BIT5;

    //sample speed, sample and hold = 16, on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    //sample res = 14 bit
    ADC14->CTL1 = ADC14_CTL1_RES_3;

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_0;

    // int enable
    ADC14->IER0 |= ADC14_IER0_IE0;

    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);

    //wake on isr exit
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

}

void ADC_RequestNextSample() {
    //start sample
    if (adcflag != F_ADC_REQUEST) {
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
    }
    adcflag = F_ADC_REQUEST;
}

void ADC14_IRQHandler() {
    lastRead = ADC14->MEM[0]; //output

    adcflag =F_ADC_READ_ME;
}

int ADC_CheckReady() {
    return adcflag == F_ADC_READ_ME;
}

unsigned int ADC_GetRawValue() {
    adcflag = F_ADC_NO_OP;
    return lastRead;
}

void ADC_GetFormatedValue(char* value) {
    adcflag = F_ADC_NO_OP;
    unsigned long long conversion = lastRead * CAL;
    int loc = 5;
    value[2] = '.';
    while (loc >= 0) {
        value[loc] = '0' + (conversion % 10);
        conversion /= 10;
        if (loc == 3) loc--;
        loc--;
    }
}
