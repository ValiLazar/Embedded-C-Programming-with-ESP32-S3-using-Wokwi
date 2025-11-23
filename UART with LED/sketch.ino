#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h" 

#define UART_NUM UART_NUM_0
#define BUF_SIZE 1024  

void initUART() {
    uart_config_t uart_config = {};
    uart_config.baud_rate = 115200;          
    uart_config.data_bits = UART_DATA_8_BITS; 
    uart_config.parity    = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;

    uart_param_config(UART_NUM, &uart_config);

    uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM, BUF_SIZE, 0, 0, NULL, 0);
}

void setup() {
    gpio_reset_pin(GPIO_NUM_1);
    gpio_set_direction(GPIO_NUM_1, GPIO_MODE_OUTPUT);

    initUART();

    printf("Consola Pornita. Scrie ceva si da Enter!\n");
}

void loop() {

    uint8_t data[128]; 
    
    int length = uart_read_bytes(UART_NUM, data, 128, 20 / portTICK_PERIOD_MS);

    if (length > 0) {
        
        data[length] = '\0'; 

        printf("Am primit comanda: %s\n", data);

        if (strncmp((char*)data, "on", 2) == 0) {
            printf("-> Execut: APRINDE LED\n");
            gpio_set_level(GPIO_NUM_1, 1);
        }
        else if (strncmp((char*)data, "off", 3) == 0) {
            printf("-> Execut: STINGE LED\n");
            gpio_set_level(GPIO_NUM_1, 0);
        }
        else {
            printf("-> Eroare: Comanda necunoscuta!\n");
        }
    }
}