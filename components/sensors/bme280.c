//
// Created by yann on 11/19/24.
//

#include "bme280.h"

#include <esp_log.h>
#include <driver/i2c.h>

#include "bmx280.h"

#define SDA_PIN GPIO_NUM_16
#define SCL_PIN GPIO_NUM_17

char* const TAG = "bme280";

struct bme_sensor {
    bmx280_t* bmx280;
};

bme_sensor_t* bme280_init() {
    bme_sensor_t* sensor = malloc(sizeof(bme_sensor_t));

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

    ESP_ERROR_CHECK(
        i2c_param_config(I2C_NUM_0, &i2c_config)
    );

    ESP_ERROR_CHECK(
        i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0)
    );

    sensor->bmx280 = bmx280_create(I2C_NUM_0);

    if (!sensor->bmx280) {
        ESP_LOGE(TAG, "Could not create bmp280 driver");
        free(sensor);
    }

    ESP_ERROR_CHECK(
        bmx280_init(sensor->bmx280)
    );

    ESP_ERROR_CHECK(
        bmx280_configure(sensor->bmx280, &BMX280_DEFAULT_CONFIG)
    );

    return sensor;
}

void bme280_read_sensor_values(const bme_sensor_t* sensor, float* temperature, float* pressure, float* humidity) {
    ESP_ERROR_CHECK(
        bmx280_setMode(sensor->bmx280, BMX280_MODE_FORCE)
    );
    do {
        vTaskDelay(pdMS_TO_TICKS(1));
    } while (bmx280_isSampling(sensor->bmx280));

    bmx280_readoutFloat(sensor->bmx280, temperature, pressure, humidity);
}
