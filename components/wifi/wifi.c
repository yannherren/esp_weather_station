//
// Created by yann on 11/21/24.
//

#include "wifi.h"
#include <esp_log.h>

#include "esp_wifi.h"
#include "sdkconfig.h"

void wifi_init() {
    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(
        esp_wifi_init(&config)
    );
}

void wifi_connect() {
    wifi_config_t config = {
        .sta = {
            .ssid = CONFIG_WIFI_WRAPPER_SSID,
            .password = CONFIG_WIFI_WRAPPER_PASSWORD
        }
    };

    ESP_ERROR_CHECK(
        esp_wifi_set_mode(WIFI_MODE_STA)
    );

    ESP_ERROR_CHECK(
        esp_wifi_set_config(WIFI_IF_STA, &config)
    );

    ESP_ERROR_CHECK(
        esp_wifi_start()
    );

    ESP_ERROR_CHECK(
        esp_wifi_connect()
    );
}

void wifi_stop() {
    ESP_ERROR_CHECK(
        esp_wifi_disconnect()
    );
    ESP_ERROR_CHECK(
        esp_wifi_stop()
    );
    ESP_ERROR_CHECK(
        esp_wifi_deinit()
    );
}
