//
// Created by yann on 12/6/24.
//

#ifndef BME280_PRIVATE_H
#define BME280_PRIVATE_H
#include <esp_err.h>

typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    uint16_t dig_H1;
    int16_t dig_H2;
    uint16_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int16_t dig_H6;
} calibration_data_t;

static esp_err_t bme280_load_chip_id(uint8_t *chip_id);

static void bme280_read_raw_data(uint8_t *data, uint8_t start_register, uint8_t len);

static uint32_t bme280_compose_temp(uint8_t *raw_data);
static uint32_t bme280_compose_pressure(uint8_t *raw_data);
static uint32_t bme280_compose_humidity(uint8_t *raw_data);

static double bme280_compensate_temp(calibration_data_t *calibration_data, int32_t adc_T, double *t_fine);
static double bme280_compensate_pressure(calibration_data_t *calibration_data, int32_t adc_P, double t_fine);
static double bme280_compensate_humidity(calibration_data_t *calibration_data, int32_t adc_H, double t_fine);

static void bme280_load_calibration_data(calibration_data_t *calibration_data);


#endif //BME280_PRIVATE_H
