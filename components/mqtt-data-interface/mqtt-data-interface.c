//
// Created by yann on 11/22/24.
//

#include "mqtt-data-interface.h"

#include <esp_log.h>

#include "mqtt_client.h"
#include "sdkconfig.h"

ESP_EVENT_DEFINE_BASE(MQTT_EVENT);


esp_mqtt_client_handle_t data_init() {
    const esp_mqtt_client_config_t config = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URI,
        .credentials.username = CONFIG_MQTT_USER,
        .credentials.client_id = CONFIG_MQTT_CLIENT_ID,
        .credentials.authentication.password = CONFIG_MQTT_PASSWORD
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&config);

    return client;
}

void data_register_event(esp_mqtt_client_handle_t client, void (*event_handler) (void* event_handler_arg,
                                        esp_event_base_t event_base,
                                        int32_t event_id,
                                        void* event_data)) {
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, event_handler, client);
}

void data_client_start(esp_mqtt_client_handle_t client) {
    ESP_ERROR_CHECK(
         esp_mqtt_client_start(client)
     );
}

void data_store(esp_mqtt_client_handle_t client, DATA_TYPE type, double value) {
    const char topic[] = "channels/2759051/publish/fields/field1";
    char data[64];
    snprintf(data, sizeof(data), "%lf", value);

    ESP_LOGI("STORE", "data: %s", data);

    ESP_ERROR_CHECK(
        esp_mqtt_client_publish(client, topic, data, 0, 0, 0)
    );

    ESP_LOGI("MQTT", "Sent!");
}

void data_store_bulk(float value[]) {
}
