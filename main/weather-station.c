#include <esp_log.h>
#include <nvs_flash.h>

#include "driver/gpio.h"
#include "bme280.h"
#include "wifi.h"

#define DHT_PIN GPIO_NUM_4
#define SENSOR_TYPE DHT_TYPE_DHT11


void app_main(void)
{
    nvs_flash_init();

    float temperature, pressure, humidity = 0;

    const bme_sensor_t* sensor = bme280_init();
    bme280_read_sensor_values(sensor, &temperature, &pressure, &humidity);

    ESP_LOGI("BME280", "Temperature: %f", temperature);
    ESP_LOGI("BME280", "Pressure: %f", pressure);
    ESP_LOGI("BME280", "Humidity: %f", humidity);

    wifi_init();
    // wifi_connect(wifi_handle_connected);
}
