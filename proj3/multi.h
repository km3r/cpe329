/*
 * multi.h
 *
 *  Created on: May 18, 2017
 *      Author: Kyle
 */

#ifndef MULTI_H_
#define MULTI_H_

#define BAR_RAW_VAL    305

char dc[8];
char rmst[8];
char calc[8];
char vpp[8];
char freqt[8];
char dc_s[8];
char rms_s[8];
char calc_s[8];
char vpp_s[8];
char freq_s[8];

void Refresh_Display();
void Draw_Background();
void Store();
void Setup_Multi();

#endif /* MULTI_H_ */
