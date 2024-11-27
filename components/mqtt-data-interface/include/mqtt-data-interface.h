//
// Created by yann on 11/22/24.
//

#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H
#include <mqtt_client.h>

typedef enum {
    TEMPERATURE = 1,
    HUMIDITY = 2,
    PRESSURE = 3,
    UV_INDEX = 4,
    CO2 = 5,
    TVOC = 6
} DATA_TYPE ;

esp_mqtt_client_handle_t data_init();
void data_register_event(esp_mqtt_client_handle_t client, void (*event_handler) (void* event_handler_arg,
                                        esp_event_base_t event_base,
                                        int32_t event_id,
                                        void* event_data));
void data_client_start(esp_mqtt_client_handle_t client);
void data_store(esp_mqtt_client_handle_t client, DATA_TYPE type, double value);
void data_store_bulk(float value[]);

#endif //DATA_INTERFACE_H
