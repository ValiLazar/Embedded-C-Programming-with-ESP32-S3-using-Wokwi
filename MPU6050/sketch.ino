#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define PIN_SDA 42
#define PIN_SCL 41
#define I2C_PORT I2C_NUM_0
#define MPU_ADDR 0x68 

#define REG_PWR_MGMT_1   0x6B 

#define REG_ACCEL_XOUT_H 0x3B 
#define REG_ACCEL_YOUT_H 0x3D 
#define REG_ACCEL_ZOUT_H 0x3F 

#define REG_TEMP_OUT_H   0x41 

#define REG_GYRO_XOUT_H  0x43 
#define REG_GYRO_YOUT_H  0x45 
#define REG_GYRO_ZOUT_H  0x47 

class MPU6050 {
public:
    MPU6050() {
        initI2C();
        wakeUp();
    }

    void initI2C() {
        i2c_config_t conf = {};
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = PIN_SDA;
        conf.scl_io_num = PIN_SCL;
        conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
        conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
        conf.master.clk_speed = 100000; 
        
        i2c_param_config(I2C_PORT, &conf);
        i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
    }

    void wakeUp() {
        uint8_t data[2] = {REG_PWR_MGMT_1, 0x00};
        i2c_master_write_to_device(I2C_PORT, MPU_ADDR, data, 2, 1000 / portTICK_PERIOD_MS);
    }

    int16_t readRegister16(uint8_t reg) {
        uint8_t data[2];
        i2c_master_write_read_device(I2C_PORT, MPU_ADDR, &reg, 1, data, 2, 1000 / portTICK_PERIOD_MS);
        return (data[0] << 8) | data[1];
    }

    float getTemp() {
        int16_t raw = readRegister16(REG_TEMP_OUT_H);
        return (raw / 340.0) + 36.53;
    }

    int16_t getAccelX() { return readRegister16(REG_ACCEL_XOUT_H); }
    int16_t getAccelY() { return readRegister16(REG_ACCEL_YOUT_H); }
    int16_t getAccelZ() { return readRegister16(REG_ACCEL_ZOUT_H); }

    int16_t getGyroX() { return readRegister16(REG_GYRO_XOUT_H); }
    int16_t getGyroY() { return readRegister16(REG_GYRO_YOUT_H); }
    int16_t getGyroZ() { return readRegister16(REG_GYRO_ZOUT_H); }
};

void setup() {
    MPU6050 senzor;
    printf("MPU6050:");

    while(true) {
        // Citim tot
        float t = senzor.getTemp();
        
        int16_t ax = senzor.getAccelX();
        int16_t ay = senzor.getAccelY();
        int16_t az = senzor.getAccelZ();

        int16_t gx = senzor.getGyroX();
        int16_t gy = senzor.getGyroY();
        int16_t gz = senzor.getGyroZ();

        printf("T:%.1f | Acc[X:%d Y:%d Z:%d] | Gyro[X:%d Y:%d Z:%d]\n", t, ax, ay, az, gx, gy, gz);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void loop() {}