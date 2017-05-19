/*
 * adc.c
 *
 *  Created on: May 10, 2017
 *      Author: kmrosent
 */

#include "adc.h"
#include "msp.h"
#include <math.h>

void Setup_ADC(int v_h, int v_l) {

    int i;

    vL = v_l;
    vH = v_h;
    adcflag = F_ADC_NO_OP;
    pos = 0;
    avg = 0;


    upCount = 0;
    dCount = 0;
    freqAvg = 0;
    freqPos = 0;

    for (i = 0; i < NUM_FREQ; i++) {
        freq[i] = 0;
    }

    for (i = 0; i < SAMPLES; i++) {
        lastRead[i] = 0;
    }

    P5->SEL0 |= BIT5;
    P5->SEL1 |= BIT5;

    //sample speed, sample and hold = 16, on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    //sample res = 14 bit
    ADC14->CTL1 = ADC14_CTL1_RES_3;

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_0;

    // int enable
    ADC14->IER0 |= ADC14_IER0_IE0;

    NVIC_EnableIRQ(ADC14_IRQn);

    //wake on isr exit
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

}

void ADC_RequestNextSample() {
    //start sample
    //if (adcflag != F_ADC_REQUEST) {
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
    //}
    adcflag = F_ADC_REQUEST;
}

void ADC14_IRQHandler() {
    avg -= lastRead[pos];
    rms -= (lastRead[pos]/10) * (lastRead[pos]/10);
    lastRead[pos] = ADC14->MEM[0]; //output
    avg += lastRead[pos];
    rms += (lastRead[pos]/10) * (lastRead[pos]/10);
    int prev = pos - 1;

    if (max[1] < lastRead[pos]) max[1] = lastRead[pos];
    if (min[1] > lastRead[pos]) min[1] = lastRead[pos];

    if (pos == 0) {
        prev = 3999;
        max[0] = max[1];
        min[0] = min[1];
        max[1] = lastRead[pos];
        min[1] = lastRead[pos];
    }

    //in up mode
    if (upCount > 0) {
        if (lastRead[pos] + THRESH > lastRead[prev]) {
            upCount++;
        } else {
            freqAvg -= freq[freqPos];
            freq[freqPos] = upCount;
            freqAvg += freq[freqPos];
            upCount = 0;
            dCount = 1;
            freqPos = ( freqPos + 1 ) % NUM_FREQ;
        }
    } else { //down mode
        if (lastRead[pos] < lastRead[prev] + THRESH) {
            dCount++;
        } else {
            freqAvg -= freq[freqPos];
            freq[freqPos] = dCount;
            freqAvg += freq[freqPos];
            dCount = 0;
            upCount = 1;
            freqPos = ( freqPos + 1 ) % NUM_FREQ;
        }
    }
    pos = ( pos + 1 ) % SAMPLES;

    adcflag =F_ADC_READ_ME;
}

int ADC_CheckReady() {
    return adcflag == F_ADC_READ_ME;
}

unsigned int ADC_GetRawValue() {
    adcflag = F_ADC_NO_OP;
    return avg / SAMPLES;
}

unsigned int ADC_GetRawValueAC() {
    adcflag = F_ADC_NO_OP;
    return sqrt(rms / SAMPLES);
}

unsigned int ADC_GetRawValueOhm() {
    adcflag = F_ADC_NO_OP;
    int v = lastRead[pos - 1 > 0 ? pos -1 : 3999] * CAL;
    return ((3300 + v) * RESISTOR) / v;
}

void ADC_GetFormatedDC(char* value) {
    adcflag = F_ADC_NO_OP;
    unsigned long long conversion = avg / SAMPLES * CAL /10;
    int loc = 5;
    value[2] = '.';
    while (loc >= 0) {
        value[loc] = '0' + (conversion % 10);
        conversion /= 10;
        if (loc == 3) loc--;
        loc--;
    }
}

void ADC_GetFormatedAC(char* value) {
    adcflag = F_ADC_NO_OP;
    unsigned long long conversion = (rms / SAMPLES * CAL);
    conversion = sqrt(conversion) * 10;
    int loc = 5;
    value[2] = '.';
    while (loc >= 0) {
        value[loc] = '0' + (conversion % 10);
        conversion /= 10;
        if (loc == 3) loc--;
        loc--;
    }
}

void ADC_GetFormatedAC_Calc(char* value) {
    adcflag = F_ADC_NO_OP;
    unsigned long long conversion = (rms / SAMPLES * CAL);
    conversion = sqrt(conversion) * 10 - (avg / SAMPLES * CAL);
    int loc = 5;
    value[2] = '.';
    while (loc >= 0) {
        value[loc] = '0' + (conversion % 10);
        conversion /= 10;
        if (loc == 3) loc--;
        loc--;
    }
}

void ADC_GetFormatedVpp(char* value) {
    adcflag = F_ADC_NO_OP;
    unsigned long long conversion = (max[0] - min[0]) * CAL /10;
    int loc = 5;
    value[2] = '.';
    while (loc >= 0) {
        value[loc] = '0' + (conversion % 10);
        conversion /= 10;
        if (loc == 3) loc--;
        loc--;
    }
}

void ADC_GetFormatedFreq(char* value) {
    value[0] = ' ';
    value[1] = ' ';
    value[2] = ' ';
    adcflag = F_ADC_NO_OP;
    unsigned long long conversion = 40000/ freqAvg;
    if (conversion < 250) conversion++;
    int loc = 3;
    while (loc >= 0 && conversion > 0) {
        value[loc] = '0' + (conversion % 10);
        conversion /= 10;
        loc--;
    }
}int maxCount;

void ADC_GetFormatedOhm(char* value) {
    value[0] = ' ';
    value[1] = ' ';
    value[2] = ' ';
    adcflag = F_ADC_NO_OP;
    long long v = lastRead[pos == 0 ? 3999 : pos - 1] * CAL;

    unsigned long long conversion =  ((3300 + v) * RESISTOR) / (v + 1) * 25 / 208000;
    int loc = 5;
    while (loc >= 0 && conversion > 0) {
        value[loc] = '0' + (conversion % 10);
        conversion /= 10;
        loc--;
    }
}
