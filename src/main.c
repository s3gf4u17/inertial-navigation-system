#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "DRIVER_BMP280.h"
#include "RPIPICO_TOOLS.h"
// #include "driver_MPU6050.c"

#define bufferSize 61

int main() {
    // SETUP - INITIALIZE SENSORS
    stdio_init_all(); blinkOff();
    struct bmp280_conf bmp280conf;
    struct bmp280_data bmp280data;
    bmp280_init(&bmp280conf);
    // SETUP - READ SAMPLE DATA
    sleep_ms(2000); blinkOn();
    float tm_data[bufferSize];
    for (int i=0;i<bufferSize;i++) {
        bmp280_read(&bmp280data,&bmp280conf);
        tm_data[i]=bmp280data.t;
        sleep_ms(50);
    } blinkOff();
    // ANALYSIS - READ LIVE DATA
    while (1) {
        bmp280_read(&bmp280data,&bmp280conf);
        moveValues(tm_data,bufferSize,bmp280data.t);
        // ANALYSIS - CALCULATE LOWESS
        float tm_s = processing_LOWESS(tm_data,bufferSize,6);
        printf("%.4f %.4f\n",bmp280data.t,tm_s);

        // reasonable delay
        sleep_ms(50); // 20 measurements in single second
    }

    return 0;
}