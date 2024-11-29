//
// Created by yann on 11/22/24.
//

#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H
#include <mqtt_client.h>

#include "sensor-readings.h"

typedef enum {
    TEMPERATURE = 1,
    HUMIDITY = 2,
    PRESSURE = 3,
    UV_INDEX = 4,
    CO2 = 5,
    TVOC = 6
} DATA_TYPE ;

void data_init();
void data_store(esp_mqtt_client_handle_t client, DATA_TYPE type, double value);
void data_store(esp_mqtt_client_handle_t client, DATA_TYPE type, double value);
void data_store_bulk(esp_mqtt_client_handle_t client, sensor_data_t data);

#endif //DATA_INTERFACE_H
