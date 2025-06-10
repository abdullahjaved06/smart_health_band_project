#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include "max30101.h"

LOG_MODULE_REGISTER(max30101, LOG_LEVEL_INF);

// This gets the MAX30101 device from the devicetree
const struct device *max_dev = DEVICE_DT_GET(DT_NODELABEL(max30101));

// MAX30101 initialization function
int max30101_init(void)
{
    if (!device_is_ready(max_dev)) {
        LOG_ERR("MAX30101 sensor not ready");
        return -ENODEV;
    }

    LOG_INF("MAX30101 initialized");
    // Here you would add code to initialize the MAX30101 sensor
    // such as configuring registers for heart rate, SpO2, etc.
    return 0;
}

// Fetch sample data from MAX30101 sensor
int max30101_sample_fetch(const struct device *dev)
{
    // This function fetches the latest sample from MAX30101
    // For now, assume I2C communication happens here to get raw data
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample from MAX30101");
        return -EIO;
    }
    return 0;
}

// Get heart rate data from the MAX30101 sensor
int max30101_read_heart_rate(struct sensor_value *hr)
{
    struct sensor_value raw_hr;

    // Sample data from MAX30101
    if (max30101_sample_fetch(max_dev) != 0) {
        return -EIO;
    }

    // Here, we would process the raw sensor data to extract heart rate
    // For simplicity, assume we already have the processed value
    raw_hr.val1 = 75; // Example: Heart rate of 75 bpm
    raw_hr.val2 = 0;  // No decimal portion

    *hr = raw_hr; // Assign to the provided pointer
    return 0;
}

// Get SpO2 data from the MAX30101 sensor
int max30101_read_spo2(struct sensor_value *spo2)
{
    struct sensor_value raw_spo2;

    // Sample data from MAX30101
    if (max30101_sample_fetch(max_dev) != 0) {
        return -EIO;
    }

    // Process and fetch SpO2 (assuming raw data processing)
    raw_spo2.val1 = 98; // Example: SpO2 of 98%
    raw_spo2.val2 = 0;  // No decimal portion

    *spo2 = raw_spo2; // Assign to the provided pointer
    return 0;
}
