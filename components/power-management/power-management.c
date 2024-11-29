//
// Created by yann on 11/29/24.
//

#include "power-management.h"

#include <esp_log.h>

#include "esp_sleep.h"
#include "sdkconfig.h"
#include "wifi.h"

#define POWER_MANAGEMENT_TAG "POWER MANAGEMENT"

void power_management_sleep_and_wakeup() {
    wifi_stop();
    const int wakeup_interval = CONFIG_POWER_MANAGEMENT_SLEEP_INTERVAL;
    ESP_LOGI(POWER_MANAGEMENT_TAG, "Entering sleep for %i seconds", wakeup_interval);
    ESP_ERROR_CHECK(
        esp_sleep_enable_timer_wakeup(wakeup_interval * 1000000)
    );
    esp_deep_sleep_start();
}

