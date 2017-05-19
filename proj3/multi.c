/*
 * multi.c
 *
 *  Created on: May 18, 2017
 *      Author: Kyle
 */

#include "multi.h"
#include "uart.h"
#include "adc.h"

void Draw_Background() {

    UART_String("\033[2J", 4, 0);
    UART_String("\033[H", 3, 0);
    UART_String("------------------------------------------------------------", 60, 1);
    UART_String("|                    Digital Multimeter                    |", 60, 1);
    UART_String("|__________________________________________________________|", 60, 1);
    UART_String("|   DC-V   ||  TrueRms ||  CalcRms ||   V-pp   ||   Freq   |", 60, 1);
    UART_String("|----------------------------------------------------------|", 60, 1);
    UART_String("|          ||          ||          ||          ||          |", 60, 1);
    UART_String("|----------------------------------------------------------|", 60, 1);
    UART_String("|  DC Meter                                                |", 60, 1);
    UART_String("| 0.0V   | 0.5V   | 1.0V   | 1.5V   | 2.0V   | 2.5V   | 3V |", 60, 1);
    UART_String("|                                                          |", 60, 1);
    UART_String("|----------------------------------------------------------|", 60, 1);
    UART_String("|  Calc-RMS Meter                                          |", 60, 1);
    UART_String("| 0.0V   | 0.5V   | 1.0V   | 1.5V   | 2.0V   | 2.5V   | 3V |", 60, 1);
    UART_String("|                                                          |", 60, 1);
    UART_String("|----------------------------------------------------------|", 60, 1);
    UART_String("| Resistance:                                              |", 60, 1);
    UART_String("|__________________________________________________________|", 60, 1);

//    todo resistor maybe?
}

void Refresh_Display() {
    char arr[8];
    int i,j;
    //DC
    UART_String("\033[6;4H", 6, 0);
    ADC_GetFormatedDC(arr);
    for (i = 1; i < 6; i++) {
        UART0Tx(arr[i]);
    }
    UART0Tx('V');
    //AC - true
    UART_String("\033[6;16H", 7, 0);
    ADC_GetFormatedAC(arr);
    for (i = 1; i < 6; i++) {
        UART0Tx(arr[i]);
    }
    UART0Tx('V');
    //AC - calc
    UART_String("\033[6;28H", 7, 0);
    ADC_GetFormatedAC_Calc(arr);
    for (i = 1; i < 6; i++) {
        UART0Tx(arr[i]);
    }
    UART0Tx('V');
    //V peak to peak
    UART_String("\033[6;40H", 7, 0);
    ADC_GetFormatedVpp(arr);
    for (i = 1; i < 6; i++) {
        UART0Tx(arr[i]);
    }
    UART0Tx('V');

    //freq
    UART_String("\033[6;52H", 7, 0);
    ADC_GetFormatedFreq(arr);
    for (i = 0; i < 4; i++) {
        UART0Tx(arr[i]);
    }
    UART0Tx('H');
    UART0Tx('z');



    //DC BAR
    UART_String("\033[10;2H", 7, 0);
    i = ADC_GetRawValue();
    j = i / BAR_RAW_VAL;
    while (i--) {
        UART0Tx('\262');
    }
    i = i - j * BAR_RAW_VAL;
    if (i > 202)
        UART0Tx('\261');
    else if (i > 101)
        UART0Tx('\260');

    //AC BAR
    UART_String("\033[14;2H", 7, 0);
    i = ADC_GetRawValueAC();
    j = i / BAR_RAW_VAL;
    while (i--) {
        UART0Tx('\262');
    }
    i = i - j * BAR_RAW_VAL;
    if (i > 202)
        UART0Tx('\261');
    else if (i > 101)
        UART0Tx('\260');


    //Resistance
    UART_String("\033[16;15H", 8, 0);
    ADC_GetFormatedOhm(arr);
    for (i = 0; i < 6; i++) {
        UART0Tx(arr[i]);
    }
    UART0Tx('\352');
}
