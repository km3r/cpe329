/*
 * sound.h
 *
 *  Created on: Jun 2, 2017
 *      Author: kmrosent
 */

#ifndef SOUND_H_
#define SOUND_H_

#define SET_WAVE(num,freq,period) wave[num][1] = freq; wave[num][2] = period;
#define NEXT_WAVE(num) wave[num][0] = ((wave[num][0] + wave[num][1] ) % (256 * wave[num][1]))

int wave[4][3];



void Sound_Init(int,int,int,int);

void Update_Wave(int waveNum, int freq, int period);
void Update_Period(int waveNum, int period);
void Update_Freq(int waveNum, int freq);

void Sound_Go();









#endif /* SOUND_H_ */
