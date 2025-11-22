#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

class Button {
private:
    gpio_num_t pin;
    int lastState;
public:
    Button(int p) {
        pin = (gpio_num_t)p;
        gpio_reset_pin(pin);
        gpio_set_direction(pin, GPIO_MODE_INPUT);
        gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
        lastState = 1;
    }
    bool wasPressed() {
        int currentState = gpio_get_level(pin);
        bool click = false;
        if (currentState == 0 && lastState == 1) click = true;
        lastState = currentState;
        return click;
    }
};

class DimmerLed {
private:
    gpio_num_t pin;
    ledc_channel_t channel;
    ledc_timer_t timer;
public:
    DimmerLed(int p, int ch) {
        pin = (gpio_num_t)p;
        channel = (ledc_channel_t)ch;
        timer = LEDC_TIMER_0;
        configurareHardware();
    }
    void configurareHardware() {
        ledc_timer_config_t timer_conf = {};
        timer_conf.speed_mode = LEDC_LOW_SPEED_MODE;
        timer_conf.timer_num  = timer;
        timer_conf.duty_resolution = LEDC_TIMER_13_BIT;
        timer_conf.freq_hz    = 5000;
        timer_conf.clk_cfg    = LEDC_AUTO_CLK;
        ledc_timer_config(&timer_conf);

        ledc_channel_config_t channel_conf = {};
        channel_conf.gpio_num   = pin;
        channel_conf.speed_mode = LEDC_LOW_SPEED_MODE;
        channel_conf.channel    = channel;
        channel_conf.intr_type  = LEDC_INTR_DISABLE;
        channel_conf.timer_sel  = timer;
        channel_conf.duty       = 0;
        channel_conf.hpoint     = 0;
        ledc_channel_config(&channel_conf);
    }
    void set(int procent) {
        if (procent > 100) procent = 100;
        int duty = (8191 * procent) / 100;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
    }
};

void setup() {
    DimmerLed ledRosu(42, 0);
    DimmerLed ledVerde(41, 1);
    DimmerLed ledAlbastru(40, 2);
    
    Button buton1(2);
    Button buton2(35);

    int modCuloare = 0;

    printf("RGB Mixer Start\n");

    while(true) {
        if (buton1.wasPressed()) {
            modCuloare++; 
            if (modCuloare > 6) modCuloare = 0;

            printf("Mod Culoare: %d\n", modCuloare);

            switch (modCuloare) {
                case 0:
                    ledRosu.set(0); ledVerde.set(0); ledAlbastru.set(0);
                    break;
                case 1:
                    ledRosu.set(100); ledVerde.set(0); ledAlbastru.set(0);
                    break;
                case 2:
                    ledRosu.set(0); ledVerde.set(100); ledAlbastru.set(0);
                    break;
                case 3:
                    ledRosu.set(0); ledVerde.set(0); ledAlbastru.set(100);
                    break;
                case 4:
                    ledRosu.set(50); ledVerde.set(0); ledAlbastru.set(50);
                    break;
                case 5:
                    ledRosu.set(0); ledVerde.set(50); ledAlbastru.set(50);
                    break;
                case 6:
                    ledRosu.set(50); ledVerde.set(50); ledAlbastru.set(0);
                    break;
            }
        }

        if (buton2.wasPressed()) {
            modCuloare--; 
            if (modCuloare < 0) modCuloare = 6;

            printf("Mod Culoare: %d\n", modCuloare);

            switch (modCuloare) {
                case 0:
                    ledRosu.set(0); ledVerde.set(0); ledAlbastru.set(0);
                    break;
                case 1:
                    ledRosu.set(100); ledVerde.set(0); ledAlbastru.set(0);
                    break;
                case 2:
                    ledRosu.set(0); ledVerde.set(100); ledAlbastru.set(0);
                    break;
                case 3:
                    ledRosu.set(0); ledVerde.set(0); ledAlbastru.set(100);
                    break;
                case 4:
                    ledRosu.set(50); ledVerde.set(0); ledAlbastru.set(50);
                    break;
                case 5:
                    ledRosu.set(0); ledVerde.set(50); ledAlbastru.set(50);
                    break;
                case 6:
                    ledRosu.set(50); ledVerde.set(50); ledAlbastru.set(0);
                    break;
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void loop() {}