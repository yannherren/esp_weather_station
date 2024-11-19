//
// Created by yann on 11/19/24.
//

#ifndef BMP280_H
#define BMP280_H

typedef struct bme_sensor bme_sensor_t;

bme_sensor_t* bme280_init();
void bme280_read_sensor_values(const bme_sensor_t* sensor_config, float* temperature, float* pressure, float* humidity);

#endif //BMP280_H
