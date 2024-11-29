//
// Created by yann on 11/22/24.
//

#include "mqtt-data-interface.h"

#include <esp_log.h>

#include "mqtt_client.h"
#include "sdkconfig.h"

ESP_EVENT_DEFINE_BASE(MQTT_EVENT);

const char DATA_TAG[] = "DATA";

void data_forward_handle(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_event_post(MQTT_EVENT, event_id, event_data, sizeof(esp_mqtt_event_t), portMAX_DELAY);
}

void data_init() {
    const esp_mqtt_client_config_t config = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URI,
        .credentials.username = CONFIG_MQTT_USER,
        .credentials.client_id = CONFIG_MQTT_CLIENT_ID,
        .credentials.authentication.password = CONFIG_MQTT_PASSWORD,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, data_forward_handle, client);

    ESP_ERROR_CHECK(
     esp_mqtt_client_start(client)
    );
}

void data_store(esp_mqtt_client_handle_t client, DATA_TYPE type, double value) {
    char topicBuffer[40];
    sniprintf(topicBuffer, sizeof(topicBuffer), "channels/2759051/publish/fields/field%i", type);
    char dataBuffer[64];
    snprintf(dataBuffer, sizeof(dataBuffer), "%.2lf", value);

    ESP_ERROR_CHECK(
        esp_mqtt_client_publish(client, topicBuffer, dataBuffer, sizeof(dataBuffer), 0, 0)
    );

    vTaskDelay(1000 / portTICK_PERIOD_MS); // BUG: mqtt loses events if they are fired to quickly, qos is not available on thingspeak

    ESP_LOGI(DATA_TAG, "Stored data in field %i: %.2lf", type, value);
}

void data_store_bulk(esp_mqtt_client_handle_t client, sensor_data_t data) {
    data_store(client, TEMPERATURE, data.temperature);
    data_store(client, HUMIDITY, data.humidity);
    data_store(client, PRESSURE, data.pressure);
}
