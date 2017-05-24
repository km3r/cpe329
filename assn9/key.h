/*
 * key.h
 *
 *  Created on: May 22, 2017
 *      Author: kmrosent
 */

#ifndef KEY_H_
#define KEY_H_


typedef enum { K0 = 0, K1, K2, K3, K4, K5, K6, K7, K8, K9, K_STAR, K_POUND, NONE } Key;

void Key_init();
Key Get_key();


#endif /* KEY_H_ */
