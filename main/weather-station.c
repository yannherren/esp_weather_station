#include <esp_log.h>

#include "driver/gpio.h"
#include "sensors/bmp280.h"

#define DHT_PIN GPIO_NUM_4
#define SENSOR_TYPE DHT_TYPE_DHT11

void app_main(void)
{
    init();

}

