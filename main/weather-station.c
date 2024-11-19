#include <esp_log.h>

#include "driver/gpio.h"
#include "sensors/bme280.h"

#define DHT_PIN GPIO_NUM_4
#define SENSOR_TYPE DHT_TYPE_DHT11

void app_main(void)
{
    float temperature, pressure, humidity = 0;

    const bme_sensor_t* sensor_config = bme280_init();
    bme280_read_sensor_values(sensor_config, &temperature, &pressure, &humidity);

    ESP_LOGI("BME280", "Temperature: %f", temperature);
    ESP_LOGI("BME280", "Pressure: %f", pressure);
    ESP_LOGI("BME280", "Humidity: %f", humidity);
}
