
/*
 * assn4: reflex : main.c
 */

#include "msp.h"

#include "proj1.h"

static ul curr_freq = FREQ_3_MHz;

int main(void) {
    LCD_init();
    Key_init();
    Key key = NONE;
    Key entered[4];
    P2->OUT |= BIT5;
    P6->OUT |= BIT5;

    char arr[10];
    for(;;) {
        while ( (P2->IN & BIT5) && (P6->IN & BIT5) ) {
            //wait for a button press
        }
        Clear_LCD();
        LCD_string(" GO! ", 4);
        unsigned int x = 0;
        if ((P2->IN & BIT5)) {
            while (P2->IN & BIT5) x++;
        }
        else {
            while (P6->IN & BIT5) x++;
        }
        sprintf(arr, "%10u", x/3);
        LCD_string(arr, 10);


        delayMs(500);

    }
}

void LCD_string(char * str, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        LCD_data(str[i]);
    }
}

void LCD_key(Key retMe) {
    switch (retMe) {
        case K1:
            LCD_data('1');
            break;
        case K2:
            LCD_data('2');
            break;
        case K3:
            LCD_data('3');
            break;
        case K4:
            LCD_data('4');
            break;
        case K5:
            LCD_data('5');
            break;
        case K6:
            LCD_data('6');
            break;
        case K7:
            LCD_data('7');
            break;
        case K8:
            LCD_data('8');
            break;
        case K9:
            LCD_data('9');
            break;
        case K_STAR:
            LCD_data('*');
            break;
        case K0:
            LCD_data('0');
            break;
        case K_POUND:
            LCD_data('#');
            break;
    }
}

delay(){}

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

    //P2->DIR = 0x00;
    //P2->DIR |= 0x20;
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

    P1->DIR = 0x00;
    P1->DIR |= 0x40;
    P1->OUT &= ~0x40;
    delay();
    in = P2->IN & 0x07;
    P1->OUT |=0x40;
    if (in == 0x6)
        retMe = K9;
    if (in == 0x5)
        retMe = K8;
    if (in == 0x3)
        retMe = K7;

    //row 4
    P1->DIR = 0x00;
    P1->DIR |= 0x80;
    P1->OUT &= ~0x80;
    delay();
    in = P2->IN & 0x07;
    P1->OUT |=0x80;
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
    P6->DIR &= ~BIT5;
    P6->REN |= BIT5;

}

void LCD_init(void) {
    P4->DIR = 0xFF;     /* make P4 pins output for data and controls */
    P6->DIR = 0x01;
    delayMs(30);                /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(10);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delayMs(1);

    LCD_command(0x28);      /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* With 4-bit mode, each command or data is sent twice with upper
 * nibble first then lower nibble.
 */
void LCD_nibble_write(unsigned char data, unsigned char control) {
    data &= 0xF0;           /* clear lower nibble for control */
    control &= 0x0F;        /* clear upper nibble for data */
    P4->OUT = data | control;       /* RS = 0, R/W = 0 */
    P6->OUT = control;
    P4->OUT = data | control | EN;  /* pulse E */
    delayMs(0);
    P4->OUT = data;                 /* clear E */
    P4->OUT = 0;
    P6->OUT = 0;
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delayMs(4);         /* commands 1 and 2 need up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delayMs(1);
}


void Clear_LCD(void) {
    // send 0x01 in two nibbles
    // Write upper
    P6->OUT &= ~BIT0;
    P4->OUT = 0;
    P4->OUT |= EN;
    delayMs(0);
    P4->OUT = 0;
    //write lower
    P4->OUT = 0x10;
    P4->OUT |= EN;
    delayMs(0);
    P4->OUT &= ~EN;
    P4->OUT = 0;
}
void Home_LCD(void) {
    // send 0x02 in two nibbles
    // Write upper
    P6->OUT &= ~BIT0;
    P4->OUT = 0;
    P4->OUT |= EN;
    delayMs(0);
    P4->OUT = 0;
    //write lower
    P4->OUT = 0x20;
    P4->OUT |= EN;
    delayMs(0);
    P4->OUT &= ~EN;
    P4->OUT = 0;
}

/* delay milliseconds when system clock is at 3 MHz */
void delayMs(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 750; i > 0; i--);      /* Delay */
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
