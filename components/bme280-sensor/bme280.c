//
// Created by yann on 11/19/24.
//

#include "bme280.h"
#include "bme280_private.h"

#include <esp_log.h>
#include <driver/i2c.h>

#define TAG "bme280"

#define SDA_PIN GPIO_NUM_16
#define SCL_PIN GPIO_NUM_17

#define I2C_PORT I2C_NUM_0
#define BME_ADDRESS 0x76
#define BME_CHIP_ID 0x60
#define DEFAULT_TIMEOUT pdMS_TO_TICKS(1000)

esp_err_t bme280_load_chip_id(uint8_t *chip_id) {
    uint8_t reg_addr = 0xD0;
    uint8_t bme_chip_id = 0;
    i2c_master_write_to_device(I2C_PORT, BME_ADDRESS, &reg_addr, 1, DEFAULT_TIMEOUT);
    esp_err_t err = i2c_master_read_from_device(I2C_PORT, BME_ADDRESS, &bme_chip_id, 1, DEFAULT_TIMEOUT);

    if (bme_chip_id != BME_CHIP_ID) {
        esp_err_to_name(err);
        return err;
    }

    *chip_id = bme_chip_id;

    return ESP_OK;
}


esp_err_t bme280_init() {
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,

        .master = {
            .clk_speed = 100000
        }
    };

    ESP_ERROR_CHECK(
        i2c_param_config(I2C_PORT, &i2c_config)
    );

    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);

    uint8_t chip_id = 0;
    esp_err_t err = bme280_load_chip_id(&chip_id);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't initialize sensor!");
        esp_err_to_name(err);
        return err;
    }
    ESP_LOGI(TAG, "Initialized sensor successfully");

    return ESP_OK;
}


void bme280_configure() {
    uint8_t ctrl_hum[2] = {0xF2, 0x01}; // Oversampling x 1 for humidity
    i2c_master_write_to_device(I2C_PORT, BME_ADDRESS, ctrl_hum, sizeof(ctrl_hum), DEFAULT_TIMEOUT);

    uint8_t ctrl_meas[2] = {0xF4, 0x27}; // Oversampling x 1 for both temperature and pressure
    i2c_master_write_to_device(I2C_PORT, BME_ADDRESS, ctrl_meas, sizeof(ctrl_meas), DEFAULT_TIMEOUT);
}

void bme280_read_raw_data(uint8_t *data, uint8_t start_register, uint8_t len) {
    i2c_master_write_to_device(I2C_PORT, BME_ADDRESS, &start_register, 1, DEFAULT_TIMEOUT);
    ESP_ERROR_CHECK(
        i2c_master_read_from_device(I2C_PORT, BME_ADDRESS, data, len, pdMS_TO_TICKS(1000))
    );
}

void bme280_load_calibration_data(calibration_data_t *calibration_data) {
    uint8_t len = 26; // 0x88 until register 0x9F
    uint8_t data[len];
    uint8_t start_register = 0x88;

    bme280_read_raw_data(data, start_register, len);

    calibration_data->dig_T1 = (uint16_t) (data[1] << 8 | data[2]);
    calibration_data->dig_T2 = (int16_t) (data[3] << 8 | data[2]);
    calibration_data->dig_T3 = (int16_t) (data[5] << 8 | data[4]);
    calibration_data->dig_P1 = (int16_t) (data[7] << 8 | data[6]);
    calibration_data->dig_P2 = (int16_t) (data[9] << 8 | data[8]);
    calibration_data->dig_P3 = (int16_t) (data[11] << 8 | data[10]);
    calibration_data->dig_P4 = (int16_t) (data[13] << 8 | data[12]);
    calibration_data->dig_P5 = (int16_t) (data[15] << 8 | data[14]);
    calibration_data->dig_P6 = (int16_t) (data[17] << 8 | data[16]);
    calibration_data->dig_P7 = (int16_t) (data[19] << 8 | data[18]);
    calibration_data->dig_P8 = (int16_t) (data[21] << 8 | data[20]);
    calibration_data->dig_P9 = (int16_t) (data[23] << 8 | data[22]);
    calibration_data->dig_H1 = data[25];

    uint8_t h_len = 7; // 0xE1 until register 0xE7
    uint8_t h_data[len];
    uint8_t h_start_register = 0xE1;

    bme280_read_raw_data(h_data, h_start_register, h_len);

    calibration_data->dig_H2 = (int16_t) (h_data[1] << 8 | h_data[0]);
    calibration_data->dig_H3 = h_data[2];
    calibration_data->dig_H4 = (int16_t) (h_data[3] << 4 | h_data[4] >> 4);
    calibration_data->dig_H5 = (int16_t) (h_data[5] << 4 | (h_data[4] & 0x0F));
    calibration_data->dig_H6 = h_data[6];
}

uint32_t bme280_compose_temp(uint8_t *raw_data) {
    return raw_data[0] << 12 | raw_data[1] << 4 | raw_data[2] >> 4;
}

uint32_t bme280_compose_pressure(uint8_t *raw_data) {
    return raw_data[0] << 12 | raw_data[1] << 4 | raw_data[2] >> 4;
}

uint32_t bme280_compose_humidity(uint8_t *raw_data) {
    return raw_data[0] << 8 | raw_data[1];
}

double bme280_compensate_temp(calibration_data_t *calibration_data, int32_t adc_T, double *t_fine) {
    // Formulas for compensation can be found on: https://www.mouser.com/datasheet/2/783/BST-BME280-DS002-1509607.pdf
    double var1, var2, T;
    var1 = ((double) adc_T / 16384.0 - (double) calibration_data->dig_T1 / 1024.0) * (double) calibration_data->dig_T2;
    var2 = (((double) adc_T / 131072.0 - (double) calibration_data->dig_T1 / 8192.0) *
            ((double) adc_T / 131072.0 - (double) calibration_data->dig_T1 / 8192.0)) * (double) calibration_data->
           dig_T3;

    if (t_fine != NULL) {
        *t_fine = var1 + var2;
    }

    T = (var1 + var2) / 5120.0;

    return T;
}

double bme280_compensate_pressure(calibration_data_t *calibration_data, int32_t adc_P, double t_fine) {
    // Formulas for compensation can be found on: https://www.mouser.com/datasheet/2/783/BST-BME280-DS002-1509607.pdf
    double var1, var2, p;
    var1 = t_fine / 2.0 - 64000.0;
    var2 = var1 * var1 * (double) calibration_data->dig_P6 / 32768.0;
    var2 = var2 + var1 * (double) calibration_data->dig_P5 * 2.0;
    var2 = (var2 / 4.0) + ((double) calibration_data->dig_P4 * 65536.0);
    var1 = ((double) calibration_data->dig_P3 * var1 * var1 / 524288.0 + (double) calibration_data->dig_P2 * var1) /
           524288.0;
    var1 = (1.0 + var1 / 32768.0) * (double) calibration_data->dig_P1;
    if (var1 == 0.0) {
        return 0;
    }
    p = 1048576.0 - (double) adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double) calibration_data->dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double) calibration_data->dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double) calibration_data->dig_P7)) / 16.0;
    return p;
}

double bme280_compensate_humidity(calibration_data_t *calibration_data, int32_t adc_H, double t_fine) {
    // Formulas for compensation can be found on: https://www.mouser.com/datasheet/2/783/BST-BME280-DS002-1509607.pdf
    double var_H;
    var_H = t_fine - 76800.0;
    var_H = (adc_H - ((double) calibration_data->dig_H4 * 64.0 + (double) calibration_data->dig_H5 / 16384.0 *
                      var_H)) * ((double) calibration_data->dig_H2 / 65536.0 * (
                                     1.0 + (double) calibration_data->dig_H6 /
                                     67108864.0 * var_H *
                                     (1.0 + (double) calibration_data->dig_H3 / 67108864.0 * var_H)));
    var_H = var_H * (1.0 - (double) calibration_data->dig_H1 * var_H / 524288.0);

    if (var_H > 100.0)
        var_H = 100.0;
    else if (var_H < 0.0)
        var_H = 0.0;
    return var_H;
}

void bme280_read_data(double *temperature, double *pressure, double *humidity) {
    uint8_t raw_data[8]; // Register 0xF7 to 0xFE
    bme280_read_raw_data(raw_data, 0xF7, sizeof(raw_data));

    calibration_data_t calibration_data = {};
    bme280_load_calibration_data(&calibration_data);

    int32_t adc_P = (int32_t) bme280_compose_pressure(raw_data);
    int32_t adc_T = (int32_t) bme280_compose_temp(&raw_data[3]);
    int32_t adc_H = (int32_t) bme280_compose_humidity(&raw_data[6]);

    double t_fine = 0.0;
    *temperature = bme280_compensate_temp(&calibration_data, adc_T, &t_fine);
    *pressure = bme280_compensate_pressure(&calibration_data, adc_P, t_fine);
    *humidity = bme280_compensate_humidity(&calibration_data, adc_H, t_fine);
}
