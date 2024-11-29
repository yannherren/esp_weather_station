//
// Created by yann on 11/28/24.
//floa

#ifndef SENSOR_READINGS_H
#define SENSOR_READINGS_H


typedef struct {
    float temperature;
    float humidity;
    float pressure;
} sensor_data_t ;

sensor_data_t sensors_read_all();

#endif //SENSOR_READINGS_H
