//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include <stdint.h>
#include <math.h>

typedef double var_type ;

var_type TestFunction(var_type n);
var_type TestFunction2(var_type n);
var_type TestFunction3(var_type n);
var_type TestFunction4(var_type n);

int main(void) {
    var_type y;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    P1->SEL1 &= ~BIT0; //set P1.0 as simple I/O
    P1->SEL0 &= ~BIT0;
    P1->DIR |= BIT0; //set P1.0 as output
    P2->SEL1 &= ~(BIT2 | BIT1 | BIT0); //set P2.0-2.2 as simple I/O
    P2->SEL0 &= ~(BIT2 | BIT1 | BIT0);
    P2->DIR |= (BIT2 | BIT1 | BIT0); //set P2.0-2.2 as output pins
    //P2->OUT |= (BIT2 | BIT1 | BIT0); // turn on RGB LED
    y = TestFunction(15); // test function for timing
    //P2->OUT &= ~(BIT2 | BIT1 | BIT0); // turn off RGB LED
    y = TestFunction2(15); // test function for timing
    y = TestFunction3(15); // test function for timing
    //y = TestFunction4(15); // test function for timing
}
var_type TestFunction(var_type n) {
    var_type x;
    P1->OUT |= BIT0; // set P1.0 LED on
    {
        x = sin(n);
    }
    P1->OUT &= ~BIT0; // set P1.0 LED off
    return x;
}
var_type TestFunction2(var_type n) {
    var_type x;
    P2->OUT |= BIT0; // set P1.0 LED on
    {
        x = sinh(n);
    }
    P2->OUT &= ~BIT0; // set P1.0 LED off
    return x;
}

var_type TestFunction3(var_type n) {
    var_type x;
    P1->OUT |= BIT0; // set P1.0 LED on
    {
        x = sqrt(n);
    }
    P1->OUT &= ~BIT0; // set P1.0 LED off
    return x;
}
var_type TestFunction4(var_type n) {
    var_type x;
    P2->OUT |= BIT0; // set P1.0 LED on
    {
        x = abs(n);
    }
    P2->OUT &= ~BIT0; // set P1.0 LED off
    return x;
}
