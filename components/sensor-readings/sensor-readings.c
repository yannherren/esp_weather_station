//
// Created by yann on 11/28/24.
//

#include "sensor-readings.h"

#include <esp_event.h>

#include "bme280.h"

ESP_EVENT_DEFINE_BASE(SENSOR_READING);

sensor_data_t sensors_read_all() {
    sensor_data_t data = {
        .temperature = 0,
        .humidity = 0,
        .pressure = 0,
    };
    const bme_sensor_t* sensor = bme280_init();
    bme280_read_sensor_values(sensor, &data.temperature, &data.pressure, &data.humidity);

    return data;
}
