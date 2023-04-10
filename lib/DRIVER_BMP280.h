#ifndef DRIVER_BMP280
#define DRIVER_BMP280

#define BMP280_MISO 16
#define BMP280_CS 17
#define BMP280_SCLK 18
#define BMP280_MOSI 19
#define BMP280_PORT spi0

struct bmp280_conf {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
};

struct bmp280_data {
    float p; // pressure
    float t; // temperature
};

void bmp280_init(struct bmp280_conf* conf) {
    spi_init(BMP280_PORT,500000);
    gpio_set_function(BMP280_MISO,GPIO_FUNC_SPI);
    gpio_set_function(BMP280_SCLK,GPIO_FUNC_SPI);
    gpio_set_function(BMP280_MOSI,GPIO_FUNC_SPI);
    gpio_init(BMP280_CS);
    gpio_set_dir(BMP280_CS,GPIO_OUT);
    gpio_put(BMP280_CS,1);
    uint8_t buffer[6],reg;
    reg = 0x88 | 0x80;
    gpio_put(BMP280_CS,0);
    spi_write_blocking(BMP280_PORT,&reg,1);
    spi_read_blocking(BMP280_PORT,0,buffer,6);
    gpio_put(BMP280_CS,1);
    conf->dig_T1 = buffer[0] | (buffer[1]<<8);
    conf->dig_T2 = buffer[2] | (buffer[3]<<8);
    conf->dig_T3 = buffer[4] | (buffer[5]<<8);
    // set oversampling and power on chip
    uint8_t data[2];
    data[0] = 0xF4 & 0x7F; // remove first bit to indicate write array
    data[1] = 0x27;
    gpio_put(BMP280_CS,0);
    spi_write_blocking(BMP280_PORT,&data,2);
    gpio_put(BMP280_CS,1);
};

void bmp280_read(struct bmp280_data *data, struct bmp280_conf *conf) {
    uint8_t reg,buffer[3];
    reg = 0xFA | 0x80; // most significant bit 1 - inidicating a read request
    gpio_put(BMP280_CS,0);
    spi_write_blocking(BMP280_PORT,&reg,1);
    spi_read_blocking(BMP280_PORT,0,buffer,3); // 0 indicates no write buffer
    gpio_put(BMP280_CS,1);
    int32_t temperature_RAW = ((uint32_t) buffer[0] << 12) | ((uint32_t) buffer[1] << 4) | ((uint32_t) buffer[2] >> 4);
    int32_t var1,var2;
    var1 = ((((temperature_RAW >> 3) - ((int32_t) conf->dig_T1 << 1))) * ((int32_t) conf->dig_T2)) >> 11;
    var2 = (((((temperature_RAW >> 4) - ((int32_t) conf->dig_T1)) * ((temperature_RAW >> 4) - ((int32_t) conf->dig_T1))) >> 12) * ((int32_t) conf->dig_T3)) >> 14;
    data->t = (((var1+var2)*5+128)>>8)/100.0;
}

#endif