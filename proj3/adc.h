/*
 * adc.h
 *
 *  Created on: May 10, 2017
 *      Author: kmrosent
 */

#ifndef ADC_H_
#define ADC_H_

#define F_ADC_READ_ME 0
#define F_ADC_REQUEST 1
#define F_ADC_NO_OP   2

#define CAL         79 / 428 + 30
#define THRESH      100
#define SAMPLES     4000
#define NUM_FREQ    20
#define RESISTOR    50000

int vL, vH, adcflag;
unsigned int lastRead[SAMPLES];
unsigned int pos;
unsigned int avg;
unsigned int rms;

int freq[20];
int freqAvg;
int freqPos;
int upVal[2];
int upCount;
int dVal[2];
int dCount;
int max;
int min;



void Setup_ADC(int v_h, int v_l);
void ADC_RequestNextSample();
void ADC14_IRQHandler();

int ADC_CheckReady();
unsigned int ADC_GetRawValue();
unsigned int ADC_GetRawValueAC();
unsigned int ADC_GetRawValueOhm();
void ADC_GetFormatedDC(char* value);
void ADC_GetFormatedAC(char* value);
void ADC_GetFormatedAC_Calc(char* value);
void ADC_GetFormatedVpp(char* value);
void ADC_GetFormatedFreq(char* value);
void ADC_GetFormatedOhm(char* value);

#endif /* ADC_H_ */
