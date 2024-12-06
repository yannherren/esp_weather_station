//
// Created by yann on 11/19/24.
//

#ifndef BMP280_H
#define BMP280_H
#include <esp_err.h>

typedef struct bme_sensor bme_sensor_t;

esp_err_t bme280_init();
void bme280_configure();
void bme280_read_data(double *temperature, double *pressure, double *humidity);

#endif //BMP280_H
