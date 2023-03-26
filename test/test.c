#include <cstdint>
#include <stdio.h>

int main() {
    int32_t temp_RAW = 1048166;//920418;//920418;
    int32_t pres_RAW = 156193;//415148;

    uint16_t dig_T1 = 0;//46816;//3745;
    int16_t dig_T2 = 0;//9774;//4096;
    int16_t dig_T3 = 3832;//5666;//0;

    uint16_t dig_P1 = 0;
    int16_t dig_P2 = 0;
    int16_t dig_P3 = 0;
    int16_t dig_P4 = 0;
    int16_t dig_P5 = 0;
    int16_t dig_P6 = 28576;
    int16_t dig_P7 = 4096;
    int16_t dig_P8 = -35000;
    int16_t dig_P9 = 2080;

    // temperature
    double var1 = (((double)temp_RAW)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
    double var2 = ((((double)temp_RAW)/131072.0 - ((double)dig_T1)/8192.0) * (((double)temp_RAW)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
    double temp_REAL = (var1+var2)/5120.0;
    double temp_fine = (var1+var2);
    printf("%f\t%f\t-> %fC\t%f\n",var1,var2,temp_REAL,temp_fine);

    var1 = temp_fine/2.0-64000.0;
    var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    printf("%f\t%f\n",var1,var2);

    var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    printf("%f\n",var2);

    var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
    printf("%f\n",var2);

    var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    printf("%f\n",var1);

    var1 = (1.0 + (var1/32768.0))*((double)dig_P1);
    printf("%f\n",var1);

    double p = 1048576.0 - (double)pres_RAW;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    printf("%f\n",p);

    var1 = ((double)dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)dig_P8) / 32768.0;
    printf("%f\t%f\n",var1,var2);

    p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
    printf("%f\n",p);
}

// // 19 clk
// // 18 da