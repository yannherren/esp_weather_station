//
// Created by yann on 11/21/24.
//

#include "wifi.h"
#include <esp_log.h>
#include "esp_wifi.h"
#include "sdkconfig.h"
//
// void wifi_handle_connected() {
//     ESP_LOGI("WIFI", "YAY, connected!");
// }

void wifi_init() {
    esp_event_loop_create_default();
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(
        esp_wifi_init(&config)
    );
}

void wifi_connect(void (*event_handler)) {
    wifi_config_t config = {
        .sta = {
            .ssid = CONFIG_WIFI_WRAPPER_SSID,
            .password = CONFIG_WIFI_WRAPPER_PASSWORD
        }
    };

    ESP_ERROR_CHECK(
        esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, event_handler, NULL)
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

void wifi_disconnect() {
    ESP_ERROR_CHECK(
        esp_wifi_deinit()
    );
}
