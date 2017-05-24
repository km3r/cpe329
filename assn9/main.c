//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "key.h"
#include "servo.h"

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    Setup_Servo();
    Key_init();

    Key key;
    int angle = 0;
    while (1) {
        Update_servo(angle);
        key = Get_key();
        switch (key) {
            case K0:
                angle = 0;
                break;
            case K1:
                angle = 10;
                break;
            case K_POUND:
                angle += 10;
                continue;
                break;
            case K_STAR:
                angle -= 10;
                continue;
                break;
            default:
                continue;
        }
        key = Get_key();
        switch (key) {
            case K_POUND:
                angle += 10;
                continue;
                break;
            case K_STAR:
                angle -= 10;
                continue;
                break;
            default:
                angle += (int) key;
                angle *= 10;
                continue;
        }

    }
}
