#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "driver_BMP280.c"
#include "driver_MPU6050.c"

int main() {
    // initialize picos i/o
    stdio_init_all();

    // create needed configuration data structs
    struct conf_bmp280 confbmp280;

    // initialize sensors
    init_bmp280(&confbmp280);

    // create data structs for measurements
    struct data_bmp280 databmp280;

    // infinite loop for sensor measurement
    while (1) {
        // read sensors
        read_bmp280(&databmp280,&confbmp280);

        // print out collected data
        printf("%.4f\n",databmp280.temperature);

        // reasonable delay
        sleep_ms(100); // 10 measurements in single second
    }

    return 0;
}