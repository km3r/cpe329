/*
 * proj1.h
 *
 *  Created on: Apr 17, 2017
 *      Author: kmrosent
 */

#ifndef PROJ1_H_
#define PROJ1_H_

#define RS 1     /* P4.0 mask */
#define RW 2     /* P4.1 mask */
#define EN 4     /* P4.2 mask */

#define CYCLES_PER_LOOP 11
#define SETUP_CYCLES 5
#define FREQ 32_KHz        32768
#define FREQ_1_5_MHz     1500000
#define FREQ_3_MHz       3000000
#define FREQ_6_MHz       6000000
#define FREQ_12_MHz     12000000
#define FREQ_24_MHz     24000000
#define FREQ_48_MHz     48000000

typedef unsigned long ul;

void delayMs(int n);
void delay_ms(ul ms);
void delay_ns(ul ns);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void Key_init(void);
void Clear_LCD(void);
void Home_LCD(void);
void LCD_string(char * str, int len);



typedef enum { K1 = 0, K2, K3, K4, K5, K6, K7, K8, K9, K_STAR, K0, K_POUND, NONE } Key;
Key Get_key(void);
void LCD_key(Key retMe);
Key password[4] = {K4, K5, K4, K1};



#endif /* PROJ1_H_ */
