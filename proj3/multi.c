/*
 * multi.c
 *
 *  Created on: May 18, 2017
 *      Author: Kyle
 */

#include "multi.h"
#include "uart.h"
#include "adc.h"

void Setup_Multi() {
    int i;
    for (i = 0; i<8; i++) {
        dc_s[i] = ' ';
        rms_s[i] = ' ';
        calc_s[i] = ' ';
        vpp_s[i] = ' ';
        freq_s[i] = ' ';
    }
}

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
    UART_String("| Stored Vals:                                             |", 60, 1);
    UART_String("|__________________________________________________________|", 60, 1);
    UART_String("|   DC-V   ||  TrueRms ||  CalcRms ||   V-pp   ||   Freq   |", 60, 1);
    UART_String("|----------------------------------------------------------|", 60, 1);
    UART_String("|          ||          ||          ||          ||          |", 60, 1);
    UART_String("|__________________________________________________________|", 60, 1);

}

void Refresh_Display() {
    char arr[8];
    int i,j,k;
    //DC
    UART_String("\033[6;4H", 6, 0);
    ADC_GetFormatedDC(dc);
    for (i = 1; i < 6; i++) {
        UART0Tx(dc[i]);
    }
    UART0Tx('V');
    //AC - true
    UART_String("\033[6;16H", 7, 0);
    ADC_GetFormatedAC(rmst);
    for (i = 1; i < 6; i++) {
        UART0Tx(rmst[i]);
    }
    UART0Tx('V');
    //AC - calc
    UART_String("\033[6;28H", 7, 0);
    ADC_GetFormatedAC_Calc(calc);
    for (i = 1; i < 6; i++) {
        UART0Tx(calc[i]);
    }
    UART0Tx('V');
    //V peak to peak
    UART_String("\033[6;40H", 7, 0);
    ADC_GetFormatedVpp(vpp);
    for (i = 1; i < 6; i++) {
        UART0Tx(vpp[i]);
    }
    UART0Tx('V');

    //freq
    UART_String("\033[6;52H", 7, 0);
    ADC_GetFormatedFreq(freqt);
    for (i = 0; i < 4; i++) {
        UART0Tx(freqt[i]);
    }
    UART0Tx('H');
    UART0Tx('z');



    //DC BAR
    UART_String("\033[10;2H", 7, 0);
    i = ADC_GetRawValue();
    j = i / BAR_RAW_VAL;
    for (k = 0; k<58;k++) {
        UART0Tx(k<j ? ']' : ' ');
    }

    //AC BAR
    UART_String("\033[14;2H", 7, 0);
    i = ADC_GetRawValueAC();
    j = i / BAR_RAW_VAL;
    for (k = 0; k<58;k++) {
        UART0Tx(k<j ? ']' : ' ');
    }

    //DC -store
    UART_String("\033[20;4H", 7, 0);

    for (i = 1; i < 6; i++) {
        UART0Tx(dc_s[i]);
    }
    UART0Tx('V');
    //AC - true -store
    UART_String("\033[20;16H", 8, 0);

    for (i = 1; i < 6; i++) {
        UART0Tx(rms_s[i]);
    }
    UART0Tx('V');
    //AC - calc -store
    UART_String("\033[20;28H", 8, 0);

    for (i = 1; i < 6; i++) {
        UART0Tx(calc_s[i]);
    }
    UART0Tx('V');
    //V peak to peak -store
    UART_String("\033[20;40H", 8, 0);

    for (i = 1; i < 6; i++) {
        UART0Tx(vpp_s[i]);
    }
    UART0Tx('V');

    //freq -store
    UART_String("\033[20;52H", 8, 0);
    ADC_GetFormatedFreq(arr);
    for (i = 0; i < 4; i++) {
        UART0Tx(freq_s[i]);
    }
    UART0Tx('H');
    UART0Tx('z');
}

void Store() {
    int i;
    for (i = 0; i<8; i++) {
        dc_s[i] = dc[i];
        rms_s[i] = rmst[i];
        calc_s[i] = calc[i];
        vpp_s[i] = vpp[i];
        freq_s[i] = freqt[i];
    }
}

