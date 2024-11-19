//
// Created by yann on 11/19/24.
//

#include "bmp280.h"

#include <driver/i2c.h>

#define SDA_PIN GPIO_NUM_16
#define SCL_PIN GPIO_NUM_17

void init() {
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = false,
        .scl_pullup_en = false,

        .master = {
            .clk_speed = 100000
        }
    };

}

float read_temperature() {
}

float read_humidity() {
}

float read_pressure() {
}
