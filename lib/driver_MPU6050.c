#define MPU6050_PORT i2c1
#define MPU6050_FREQUENCY 400000
#define MPU6050_ADDRESS 0x68
#define MPU6050_SCL_PIN 19
#define MPU6050_SDA_PIN 18

struct data_mpu6050 {
    float accelerationX;
    float accelerationY;
    float accelerationZ;
    float angularX;
    float angularY;
    float angularZ;
    float temperature;
};

void init_mpu6050() {
    i2c_init(MPU6050_PORT,MPU6050_FREQUENCY); // args - i2c port, baudrate in Hz
    gpio_set_function(MPU6050_SCL_PIN,GPIO_FUNC_I2C); // set i2c pins
    gpio_set_function(MPU6050_SDA_PIN,GPIO_FUNC_I2C); // set i2c pins
    gpio_pull_up(MPU6050_SCL_PIN);
    gpio_pull_up(MPU6050_SDA_PIN);
    uint8_t reg[] = {0x6B,0x00}; // reset sensor
    i2c_write_blocking(MPU6050_PORT,MPU6050_ADDRESS,reg,2,false);
}

void read_mpu6050(struct data_mpu6050 *data) {
    uint8_t buffer[14];
    uint8_t reg = 0x3B;
    i2c_write_blocking(MPU6050_PORT,MPU6050_ADDRESS,&reg,sizeof(reg),true);
    i2c_read_blocking(MPU6050_PORT,MPU6050_ADDRESS,buffer,sizeof(buffer),false);
    data->accelerationX = ((buffer[0]<<8)|buffer[1])/16384.0;
    data->accelerationY = ((buffer[2]<<8)|buffer[3])/16384.0;
    data->accelerationZ = ((buffer[4]<<8)|buffer[5])/16384.0;
    data->temperature = ((buffer[6]<<8)|buffer[7])/340.0+36.53;
    data->angularX = ((buffer[8]<<8)|buffer[9])/250.0;
    data->angularY = ((buffer[10]<<8)|buffer[11])/250.0;
    data->angularZ = ((buffer[12]<<8)|buffer[13])/250.0;
}