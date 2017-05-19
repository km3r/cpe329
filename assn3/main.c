/*
 *  Assnment 3
 */
#include "msp.h"

#define RS 1     /* P6.0 mask */
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

void delay_ms(ul ms);
void delay_ns(ul ns);
void set_DCO(ul freq);
void Clear_LCD(void);
void Home_LCD(void);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

static ul curr_freq = FREQ_3_MHz;

void main2(void)
{

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer


    set_DCO(curr_freq);
    LCD_init();


    while (1) {

    }
}

/* delay milliseconds */
void delay_ms(ul ms) {
    ul i;
    ul num = ms * (curr_freq / 1000);
    ul itr = num / CYCLES_PER_LOOP;
    itr -= SETUP_CYCLES;
    for (i = 0; i < itr; i++);      /* Delay 1 loop*/
}
void delay_ns(ul ns) {
    ul i;
    //ul num = ns * (freq / 100000) / 10000;
    ul itr = ns * (curr_freq / 100000) / 10000 / CYCLES_PER_LOOP;
    itr -= SETUP_CYCLES;
    for (i = 0; i < itr; i++);      /* Delay 1 loop*/
}



void set_DCO(ul freq) {
    CS->KEY = CS_KEY_VAL; // unlock CS registers
    CS->CTL0 = 0; // clear register CTL0
    switch (freq) {
        case FREQ_1_5_MHz:
            CS->CTL0 = CS_CTL0_DCORSEL_0;
            break;
        case FREQ_3_MHz:
            CS->CTL0 = CS_CTL0_DCORSEL_1;
            break;
        case FREQ_6_MHz:
            CS->CTL0 = CS_CTL0_DCORSEL_2;
            break;
        case FREQ_12_MHz:
            CS->CTL0 = CS_CTL0_DCORSEL_3;
            break;
        case FREQ_24_MHz:
            CS->CTL0 = CS_CTL0_DCORSEL_4;
            break;
        case FREQ_48_MHz:
            CS->CTL0 = CS_CTL0_DCORSEL_5;
            break;
        default:
            break;
    }
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3; // select clock sources
    CS->KEY = 0; // lock the CS registers
}

void LCD_init() {
    P4->DIR = 0xFF;
    P6->DIR = 0x01;
    delay_ms(30);
    LCD_nibble_write(0x30, 0);
    delay_ms(10);
    LCD_nibble_write(0x30, 0);
    delay_ms(1);
    LCD_nibble_write(0x30, 0);
    delay_ms(1);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delay_ms(1);

    LCD_command(0x28);      /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */

}

void Clear_LCD(void) {

}
void Home_LCD(void) {

}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delay_ms(4);         /* commands 1 and 2 need up to 1.64ms */
    else
        delay_ms(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delay_ms(1);
}

void LCD_nibble_write(unsigned char data, unsigned char control) {
    data &= 0xF0;           /* clear lower nibble for control */
    control &= 0x0F;        /* clear upper nibble for data */
    P4->OUT = data | control;       /* RS = 0, R/W = 0 */
    P6->OUT = control;
    P4->OUT = data | control | EN;  /* pulse E */
    delay_ms(0);
    P4->OUT = data;                 /* clear E */
    P4->OUT = 0;
    P6->OUT = 0;
}
