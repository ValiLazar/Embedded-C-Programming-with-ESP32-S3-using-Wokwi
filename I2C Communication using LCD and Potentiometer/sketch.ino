#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_adc/adc_oneshot.h"
#include <string.h>

#define PIN_SDA 42
#define PIN_SCL 41
#define I2C_PORT I2C_NUM_0
#define I2C_FREQ 100000

class Potentiometer {
private:
    adc_oneshot_unit_handle_t handle;
    adc_channel_t channel;

    void initHardware() {
        adc_oneshot_unit_init_cfg_t init_config = {};
        init_config.unit_id = ADC_UNIT_1;
        adc_oneshot_new_unit(&init_config, &handle);

        adc_oneshot_chan_cfg_t config = {};
        config.bitwidth = ADC_BITWIDTH_DEFAULT;
        config.atten    = ADC_ATTEN_DB_12;
        adc_oneshot_config_channel(handle, channel, &config);
    }

public:
    Potentiometer(int channel_num) {
        channel = (adc_channel_t)channel_num;
        initHardware();
    }

    int read() {
        int valoare = 0;
        adc_oneshot_read(handle, channel, &valoare);
        return valoare;
    }
};

class LCD_I2C {
private:
    uint8_t addr;

    void write_byte(uint8_t val) {
        i2c_master_write_to_device(I2C_PORT, addr, &val, 1, 1000 / portTICK_PERIOD_MS);
    }

    void lcd_send_nibble(uint8_t nibble, uint8_t mode) {
        uint8_t data = (nibble & 0xF0) | mode | 0x08;
        write_byte(data);
        write_byte(data | 0x04);
        write_byte(data & ~0x04);
    }

    void lcd_send_byte(uint8_t val, uint8_t mode) {
        lcd_send_nibble(val & 0xF0, mode);
        lcd_send_nibble((val << 4) & 0xF0, mode);
    }

public:
    LCD_I2C(uint8_t address) {
        addr = address;
        initI2C();
        initLCD();
    }

    void initI2C() {
        i2c_config_t conf = {};
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = PIN_SDA;
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        conf.scl_io_num = PIN_SCL;
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        conf.master.clk_speed = I2C_FREQ;

        i2c_param_config(I2C_PORT, &conf);
        i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
    }

    void initLCD() {
        vTaskDelay(50 / portTICK_PERIOD_MS);
        lcd_send_nibble(0x30, 0);
        vTaskDelay(5 / portTICK_PERIOD_MS);
        lcd_send_nibble(0x30, 0);
        vTaskDelay(1 / portTICK_PERIOD_MS);
        lcd_send_nibble(0x30, 0);
        lcd_send_nibble(0x20, 0);

        lcd_send_byte(0x28, 0);
        lcd_send_byte(0x08, 0);
        lcd_send_byte(0x01, 0);
        vTaskDelay(2 / portTICK_PERIOD_MS);
        lcd_send_byte(0x06, 0);
        lcd_send_byte(0x0C, 0);
    }

    void print(const char* text) {
        for (int i = 0; i < strlen(text); i++) {
            lcd_send_byte(text[i], 1);
        }
    }

    void setCursor(uint8_t col, uint8_t row) {
        uint8_t row_offsets[] = {0x00, 0x40};
        lcd_send_byte(0x80 | (col + row_offsets[row]), 0);
    }

    void clear() {
        lcd_send_byte(0x01, 0);
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
};

void setup() {

    LCD_I2C lcd(0x27);
    Potentiometer volum(3); 

    lcd.setCursor(0, 0);
    lcd.print("Voltmetru Digital");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    lcd.clear();

    char buffer[16]; 

    while(true) {
        int valoare = volum.read();
        
        int procent = (valoare * 100) / 4095;

        lcd.setCursor(0, 0);
        sprintf(buffer, "Raw: %d", valoare); 
        lcd.print(buffer);

        lcd.setCursor(0, 1);
        sprintf(buffer, "Nivel: %d%%", procent);
        lcd.print(buffer);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void loop() {}