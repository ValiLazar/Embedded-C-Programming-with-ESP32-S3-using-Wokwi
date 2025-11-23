#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define PIN_MOSI 11
#define PIN_CLK  12
#define PIN_CS   10

#define REG_NOOP   0x00
#define REG_DIGIT0 0x01
#define REG_DECODEMODE  0x09
#define REG_INTENSITY   0x0A
#define REG_SCANLIMIT   0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPLAYTEST 0x0F

class MatrixSPI {
private:
    spi_device_handle_t spi;

    void send(uint8_t reg, uint8_t data) {
        uint8_t pachet[2] = {reg, data};
        
        spi_transaction_t t = {};
        t.length = 16;
        t.tx_buffer = pachet;
        
        spi_device_transmit(spi, &t);
    }

public:
    MatrixSPI() {
        initSPI();
        initMatrix();
    }

    void initSPI() {
        spi_bus_config_t buscfg = {};
        buscfg.mosi_io_num = PIN_MOSI;
        buscfg.miso_io_num = -1;
        buscfg.sclk_io_num = PIN_CLK;
        buscfg.quadwp_io_num = -1;
        buscfg.quadhd_io_num = -1;
        
        spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

        spi_device_interface_config_t devcfg = {};
        devcfg.clock_speed_hz = 1000000;
        devcfg.mode = 0;
        devcfg.spics_io_num = PIN_CS;
        devcfg.queue_size = 1;
        
        spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    }

    void initMatrix() {
        send(REG_SHUTDOWN, 1);
        send(REG_SCANLIMIT, 7);
        send(REG_DECODEMODE, 0);
        send(REG_INTENSITY, 8);
        clear();
    }

    void clear() {
        for (int i = 1; i <= 8; i++) {
            send(i, 0);
        }
    }

    void drawRow(int row, uint8_t bits) {
        send(row, bits);
    }
};

void setup() {
    printf("SPI Matrix Start\n");
    
    MatrixSPI matrix;

    uint8_t smile[8] = {
        0b00111100,
        0b01000010,
        0b10100101,
        0b10000001,
        0b10100101,
        0b10011001,
        0b01000010,
        0b00111100
    };

    for(int i=0; i<8; i++) {
        matrix.drawRow(i+1, smile[i]);
    }
}

void loop() {}