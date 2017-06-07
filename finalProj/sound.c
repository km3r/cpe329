/*
 * sound.c
 *
 *  Created on: Jun 7, 2017
 *      Author: kmrosent
 */

#include "sound.h"


int sinwaveX[256] = {960,984,1007,1031,1054,1078,1101,1124,1147,1170,1193,1216,1239,
                1261,1283,1305,1327,1349,1370,1392,1413,1433,1454,1474,1493,
                1513,1532,1551,1569,1587,1605,1622,1639,1655,1671,1687,1702,
                1717,1731,1745,1758,1771,1783,1795,1807,1817,1828,1838,1847,
                1856,1864,1872,1879,1885,1891,1897,1902,1906,1910,1913,1915,
                1917,1919,1920,1920,1920,1919,1917,1915,1913,1910,1906,1902,
                1897,1891,1885,1879,1872,1864,1856,1847,1838,1828,1817,1807,
                1795,1783,1771,1758,1745,1731,1717,1702,1687,1671,1655,1639,
                1622,1605,1587,1569,1551,1532,1513,1493,1474,1454,1433,1413,
                1392,1370,1349,1327,1305,1283,1261,1239,1216,1193,1170,1147,
                1124,1101,1078,1054,1031,1007,984,960,936,913,889,866,842,
                819,796,773,750,727,704,681,659,637,615,593,571,550,528,507,
                487,466,446,427,407,388,369,351,333,315,298,281,265,249,233,
                218,203,189,175,162,149,137,125,113,103,92,82,73,64,56,48,41,
                35,29,23,18,14,10,7,5,3,1,0,0,0,1,3,5,7,10,14,18,23,29,35,41,
                48,56,64,73,82,92,103,113,125,137,149,162,175,189,203,218,233,
                249,265,281,298,315,333,351,369,388,407,427,446,466,487,507,
                528,550,571,593,615,637,659,681,704,727,750,773,796,819,842,
                866,889,913,936};

//saw wave precalulated with 128 steps
int sawX[256] = {0,7,15,22,30,37,45,52,60,67,75,82,90,97,105,112,120,127,135,
                142,150,157,165,172,180,187,195,202,210,217,225,232,240,247,
                255,262,270,277,285,292,300,307,315,322,330,337,345,352,360,
                367,375,382,390,397,405,412,420,427,435,442,450,457,465,472,
                480,487,495,502,510,517,525,532,540,547,555,562,570,577,585,
                592,600,607,615,622,630,637,645,652,660,667,675,682,690,697,
                705,712,720,727,735,742,750,757,765,772,780,787,795,802,810,
                817,825,832,840,847,855,862,870,877,885,892,900,907,915,922,
                930,937,945,952,960,967,974,982,989,997,1004,1012,1019,1027,
                1034,1042,1049,1057,1064,1072,1079,1087,1094,1102,1109,1117,
                1124,1132,1139,1147,1154,1162,1169,1177,1184,1192,1199,1207,
                1214,1222,1229,1237,1244,1252,1259,1267,1274,1282,1289,1297,
                1304,1312,1319,1327,1334,1342,1349,1357,1364,1372,1379,1387,
                1394,1402,1409,1417,1424,1432,1439,1447,1454,1462,1469,1477,
                1484,1492,1499,1507,1514,1522,1529,1537,1544,1552,1559,1567,
                1574,1582,1589,1597,1604,1612,1619,1627,1634,1642,1649,1657,
                1664,1672,1679,1687,1694,1702,1709,1717,1724,1732,1739,1747,
                1754,1762,1769,1777,1784,1792,1799,1807,1814,1822,1829,1837,
                1844,1852,1859,1867,1874,1882,1889,1897,1904,1912};


void Sound_Init(int x1, int x2, int x3, int x4) {
    wave[0][0] = x1;
    wave[1][0] = x2;
    wave[2][0] = x3;
    wave[3][0] = x4;
    wave[0][1] = 1;
    wave[1][1] = 3;
    wave[2][1] = 4;
    wave[3][1] = 5;
    wave[0][2] = 2;
    wave[1][2] = 4;
    wave[2][2] = 6;
    wave[3][2] = 8;
}


void Update_Period(int waveNum, int period){
    Update_Wave(waveNum, 0, period);
}

void Update_Freq(int waveNum, int freq) {
    Update_Wave(waveNum, freq, 0);
}

void Update_Wave(int waveNum, int freq, int period) {
    if (waveNum >=0 && waveNum <4) {
        if (wave[waveNum][1] + freq > 0 && wave[waveNum][1] + freq <= 128)
            wave[waveNum][1] += freq;
        if (wave[waveNum][2] + period > 0 && wave[waveNum][2] + period <= 32)
            wave[waveNum][2] += period;
    }
}

void Sound_Go() {
    while (1) {
        NEXT_WAVE(0);
        NEXT_WAVE(1);
        NEXT_WAVE(2);
        NEXT_WAVE(3);
        Drive_DAC( sinwaveX[wave[0][0]%256] / (wave[0][0]/256) +
                   sinwaveX[wave[1][0]%256] / (wave[1][0]/256) +
                   sinwaveX[wave[2][0]%256] / (wave[2][0]/256) +
                   sawX[wave[3][0]%256] / (wave[3][0]/256));
    }
}
