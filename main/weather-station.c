#include <esp_log.h>
#include <esp_netif.h>
#include <esp_wifi_types.h>
#include <nvs_flash.h>

#include "driver/gpio.h"
#include "bme280.h"
#include "mqtt-data-interface.h"
#include "wifi.h"

#define DHT_PIN GPIO_NUM_4
#define SENSOR_TYPE DHT_TYPE_DHT11

ESP_EVENT_DECLARE_BASE(MQTT_EVENT);

void app_connection_changed(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_id == IP_EVENT_STA_GOT_IP) {
        data_init();
    }
}

void app_mqtt_client_changed(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_id == MQTT_EVENT_CONNECTED) {
        esp_mqtt_event_handle_t event = event_data;
        esp_mqtt_client_handle_t* client = &event->client;

        float temperature, pressure, humidity = 0;
        const bme_sensor_t* sensor = bme280_init();
        bme280_read_sensor_values(sensor, &temperature, &pressure, &humidity);

        data_store(*client, TEMPERATURE, temperature);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        data_store(*client, HUMIDITY, humidity);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        data_store(*client, PRESSURE, pressure);
    }
}


void app_main(void)
{
    nvs_flash_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &app_connection_changed, NULL, NULL);
    esp_event_handler_instance_register(MQTT_EVENT, ESP_EVENT_ANY_ID, &app_mqtt_client_changed, NULL, NULL);

    wifi_init();
    wifi_connect();


}
