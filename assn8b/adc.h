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

#define CAL     79 / 428 + 30

int vL, vH, adcflag;
unsigned long lastRead;

void Setup_ADC(int v_h, int v_l);
void ADC_RequestNextSample();
void ADC14_IRQHandler();

int ADC_CheckReady();
unsigned int ADC_GetRawValue();
void ADC_GetFormatedValue(char* value);

#endif /* ADC_H_ */
