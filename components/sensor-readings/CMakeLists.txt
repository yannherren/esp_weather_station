idf_component_register(SRCS "sensor-readings.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_event bme280-sensor
)
