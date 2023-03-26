#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define MISO 16
#define CS 17
#define SCLK 18
#define MOSI 19

#define SPI_PORT spi0

uint16_t dig_T1;
int16_t dig_T2, dig_T3;

float comp_temp(int32_t adc_T) {
    int32_t var1,var2,T;
    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) >> 14;
    T = ((var1+var2)*5+128)>>8;
    return T;
}

void read_temp_comp() {
    uint8_t buffer[6],reg;
    reg = 0x88 | 0x80;
    gpio_put(CS,0);
    spi_write_blocking(SPI_PORT,&reg,1);
    spi_read_blocking(SPI_PORT,0,buffer,6);
    gpio_put(CS,1);
    dig_T1 = buffer[0] | (buffer[1]<<8);
    dig_T2 = buffer[2] | (buffer[3]<<8);
    dig_T3 = buffer[4] | (buffer[5]<<8);
}

int main() {
    stdio_init_all();
    spi_init(SPI_PORT,500000);
    gpio_set_function(MISO,GPIO_FUNC_SPI);
    gpio_set_function(SCLK,GPIO_FUNC_SPI);
    gpio_set_function(MOSI,GPIO_FUNC_SPI);
    gpio_init(CS);
    gpio_set_dir(CS,GPIO_OUT);
    gpio_put(CS,1);

    read_temp_comp();

    // set oversampling and power on chip
    uint8_t data[2];
    data[0] = 0xF4 & 0x7F; // remove first bit to indicate write array
    data[1] = 0x27;
    gpio_put(CS,0);
    spi_write_blocking(SPI_PORT,&data,2);
    gpio_put(CS,1);

    int32_t temperature,rawtemp;
    uint8_t reg,buffer[3];

    while (1) {
        reg = 0xFA | 0x80; // most significant bit 1 - inidicating a read request
        gpio_put(CS,0);
        spi_write_blocking(SPI_PORT,&reg,1);
        spi_read_blocking(SPI_PORT,0,buffer,3); // 0 indicates no write buffer
        gpio_put(CS,1);

        rawtemp = ((uint32_t) buffer[0] << 12) | ((uint32_t) buffer[1] << 4) | ((uint32_t) buffer[2] >> 4);
        temperature = comp_temp(rawtemp);
        printf("%.2f\n",temperature/100.0);
    }

    return 0;
}

// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "pico/time.h"
// #include "hardware/gpio.h"
// #include "hardware/i2c.h"
// #include "math.h"

// #define I2C_PORT i2c1
// #define I2C_FREQUENCY 400000 // in Hz
// #define MPU6050_ADDRESS 0x68
// #define BMP280_ADDRESS 0x76

// #define SCL_PIN 19
// #define SDA_PIN 18

// struct bmp280_config {
//     uint16_t dig_T1;
//     int16_t dig_T2;
//     int16_t dig_T3;

//     uint16_t dig_P1;
//     int16_t dig_P2;
//     int16_t dig_P3;
//     int16_t dig_P4;
//     int16_t dig_P5;
//     int16_t dig_P6;
//     int16_t dig_P7;
//     int16_t dig_P8;
//     int16_t dig_P9;
// };

// void reset_mpu6050() {
//     uint8_t reg[] = {0x6B,0x00};
//     i2c_write_blocking(I2C_PORT,MPU6050_ADDRESS,reg,sizeof(reg),false);
// }

// void reset_bmp280() {
//     // uint8_t setup[] = {0xF5,((0x02<<5)|(0x05<<2))&0xFC};
//     // i2c_write_blocking(I2C_PORT, BMP280_ADDRESS, setup, sizeof(setup), false);
//     // uint8_t setup2[] = {0xF4,(0x01 << 5) | (0x03 << 2) | (0x03)};
//     // i2c_write_blocking(I2C_PORT, BMP280_ADDRESS, setup2, sizeof(setup2), false);
//     uint8_t reg[] = {0xE0,0xB6};
//     i2c_write_blocking(I2C_PORT,BMP280_ADDRESS,reg,sizeof(reg),false);
// }

// void read_mpu6050(int16_t a[3],int16_t g[3],int16_t *t) {
//     uint8_t buffer[14];
//     // read data
//     uint8_t reg = 0x3B;
//     i2c_write_blocking(I2C_PORT,MPU6050_ADDRESS,&reg,sizeof(reg),true);
//     i2c_read_blocking(I2C_PORT,MPU6050_ADDRESS,buffer,sizeof(buffer),false);
//     a[0]=(buffer[0]<<8)+buffer[1];
//     a[1]=(buffer[2]<<8)+buffer[3];
//     a[2]=(buffer[4]<<8)+buffer[5];
//     *t=(buffer[6]<<8)|buffer[7];
//     g[0]=(buffer[8]<<8)|buffer[9];
//     g[1]=(buffer[10]<<8)|buffer[11];
//     g[2]=(buffer[12]<<8)|buffer[13];
// }

// void read_bmp280(int32_t *pressure,int32_t *temperature) {
//     uint8_t buf[6];
//     uint8_t reg = 0xF7;
//     i2c_write_blocking(I2C_PORT,BMP280_ADDRESS,&reg,sizeof(reg),true);
//     i2c_read_blocking(I2C_PORT,BMP280_ADDRESS,buf,sizeof(buf),false);
//     *temperature = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
//     *pressure = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
// }

// void check_bmp280(struct bmp280_config *config) {
//     uint8_t buf[24];
//     uint8_t reg = 0x88;
//     i2c_write_blocking(I2C_PORT,BMP280_ADDRESS,&reg,sizeof(reg),false);
//     i2c_read_blocking(I2C_PORT,BMP280_ADDRESS,buf,sizeof(buf),false);
//     config->dig_T1 = (uint16_t)(buf[1] << 8) |buf[0];
//     config->dig_T2 = (int16_t)(buf[3] << 8) |buf[2];
//     config->dig_T3 = (int16_t)(buf[5] << 8) |buf[4];
//     // config->dig_P1 = (uint16_t)(buf[7] << 8) |buf[6];
//     // config->dig_P2 = (int16_t)(buf[9] << 8) |buf[8];
//     // config->dig_P3 = (int16_t)(buf[11] << 8) |buf[10];
//     // config->dig_P4 = (int16_t)(buf[13] << 8) |buf[12];
//     // config->dig_P5 = (int16_t)(buf[15] << 8) |buf[14];
//     // config->dig_P6 = (int16_t)(buf[17] << 8) |buf[16];
//     // config->dig_P7 = (int16_t)(buf[19] << 8) |buf[18];
//     // config->dig_P8 = (int16_t)(buf[21] << 8) |buf[20];
//     // config->dig_P9 = (int16_t)(buf[23] << 8) |buf[22];
// }

// int main() {
//     stdio_init_all();

//     // initialize i2c
//     i2c_init(I2C_PORT,I2C_FREQUENCY); // args - i2c port, baudrate in Hz
//     gpio_set_function(SCL_PIN,GPIO_FUNC_I2C); // set i2c pins
//     gpio_set_function(SDA_PIN,GPIO_FUNC_I2C); // set i2c pins
//     gpio_pull_up(SCL_PIN);
//     gpio_pull_up(SDA_PIN);

//     int16_t a[3],g[3],t;
//     int32_t press,temp;
//     reset_mpu6050();
//     reset_bmp280();

//     sleep_ms(2000);

//     struct bmp280_config config;
//     check_bmp280(&config);

//     sleep_ms(2000);

//     // loop requests
//     while (1) {
//         read_mpu6050(a,g,&t);
//         read_bmp280(&press,&temp);
//         // double var1 = (((double)temp)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
//         // double var2 = ((((double)temp)/131072.0 - ((double)dig_T1)/8192.0) * (((double)temp)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
//         // double temp_REAL = (var1+var2)/5120.0;
//         printf("%u\t%i\t%i\n",config.dig_T1,config.dig_T2,config.dig_T3);
//         printf("%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%i\t%i\n",a[0]/16384.0,a[1]/16384.0,a[2]/16384.0,g[0]/250.0,g[1]/250.0,g[2]/250.0,(t/340.0)+36.53,temp,press);
//         sleep_ms(100);
//     }

//     return 0;
// }