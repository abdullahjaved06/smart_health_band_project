#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "ble_service.h"
#include "tmp117.h"
#include "bmi270.h"
#include "max30101.h"  // Include MAX30101 header

LOG_MODULE_REGISTER(main_app, LOG_LEVEL_INF);

void peripheral_init(void);

void main(void)
{
    LOG_INF("Starting BLE Custom Service Application");

    // Initialize peripherals (BLE service, TMP117, BMI270, MAX30101)
    peripheral_init();

    // Main loop
    while (1) {
        k_sleep(K_SECONDS(5));

        // Reading temperature from TMP117
        struct sensor_value temp;
        if (tmp117_read_temperature(&temp) == 0) {
            LOG_INF("Temperature: %d.%06d C", temp.val1, temp.val2);

            // Convert temperature to int for BLE notification (scaled value)
            int16_t temp_centi = temp.val1 * 100 + temp.val2 / 10000;
            ble_service_notify(&temp_centi, sizeof(temp_centi));
        } else {
            LOG_WRN("Failed to read temperature from TMP117");
        }

        // Reading acceleration data from BMI270
        struct sensor_value accel_x, accel_y, accel_z;
        if (bmi270_read_acceleration(&accel_x, &accel_y, &accel_z) == 0) {
            LOG_INF("Acceleration: X=%d.%06d Y=%d.%06d Z=%d.%06d",
                    accel_x.val1, accel_x.val2,
                    accel_y.val1, accel_y.val2,
                    accel_z.val1, accel_z.val2);

            // Convert acceleration to int for BLE notification
            int32_t accel_data[3] = {
                accel_x.val1 * 100 + accel_x.val2 / 10000,
                accel_y.val1 * 100 + accel_y.val2 / 10000,
                accel_z.val1 * 100 + accel_z.val2 / 10000
            };

            // Notify BLE with acceleration data (e.g., X, Y, Z values)
            ble_service_notify(accel_data, sizeof(accel_data));
        } else {
            LOG_WRN("Failed to read acceleration data from BMI270");
        }

        // Reading gyroscope data from BMI270
        struct sensor_value gyro_x, gyro_y, gyro_z;
        if (bmi270_read_gyroscope(&gyro_x, &gyro_y, &gyro_z) == 0) {
            LOG_INF("Gyroscope: X=%d.%06d Y=%d.%06d Z=%d.%06d",
                    gyro_x.val1, gyro_x.val2,
                    gyro_y.val1, gyro_y.val2,
                    gyro_z.val1, gyro_z.val2);

            // Convert gyroscope to int for BLE notification
            int32_t gyro_data[3] = {
                gyro_x.val1 * 100 + gyro_x.val2 / 10000,
                gyro_y.val1 * 100 + gyro_y.val2 / 10000,
                gyro_z.val1 * 100 + gyro_z.val2 / 10000
            };

            // Notify BLE with gyroscope data (e.g., X, Y, Z values)
            ble_service_notify(gyro_data, sizeof(gyro_data));
        } else {
            LOG_WRN("Failed to read gyroscope data from BMI270");
        }

        // Reading Heart Rate from MAX30101
        struct sensor_value hr;
        if (max30101_read_heart_rate(&hr) == 0) {
            LOG_INF("Heart Rate: %d bpm", hr.val1);

            // Convert heart rate to int for BLE notification
            ble_service_notify(&hr.val1, sizeof(hr.val1));
        } else {
            LOG_WRN("Failed to read heart rate from MAX30101");
        }

        // Reading SpO2 from MAX30101
        struct sensor_value spo2;
        if (max30101_read_spo2(&spo2) == 0) {
            LOG_INF("SpO2: %d%%", spo2.val1);

            // Convert SpO2 to int for BLE notification
            ble_service_notify(&spo2.val1, sizeof(spo2.val1));
        } else {
            LOG_WRN("Failed to read SpO2 from MAX30101");
        }
    }
}

// Function to initialize peripherals like BLE service and sensors
void peripheral_init(void)
{
    // Initialize BLE service
    ble_service_init();

    // Initialize TMP117 sensor
    if (tmp117_init() != 0) {
        LOG_ERR("TMP117 initialization failed");
        return;
    }

    // Initialize BMI270 sensor
    if (bmi270_init() != 0) {
        LOG_ERR("BMI270 initialization failed");
        return;
    }

    // Initialize MAX30101 sensor
    if (max30101_init() != 0) {
        LOG_ERR("MAX30101 initialization failed");
        return;
    }

    LOG_INF("All peripherals initialized successfully");
}
