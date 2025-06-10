#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include "bmi270.h"

LOG_MODULE_REGISTER(bmi, LOG_LEVEL_INF);

const struct device *bmi270_dev = DEVICE_DT_GET(DT_NODELABEL(bmi270));

int bmi270_init(void)
{
    if (!device_is_ready(bmi270_dev)) {
        LOG_ERR("BMI270 device not ready");
        return -ENODEV;
    }

    LOG_INF("BMI270 initialized");
    return 0;
}

int bmi270_read_acceleration(struct sensor_value *x, struct sensor_value *y, struct sensor_value *z)
{
    if (sensor_sample_fetch(bmi270_dev) < 0) {
        LOG_ERR("Failed to fetch acceleration data");
        return -EIO;
    }

    if (sensor_channel_get(bmi270_dev, SENSOR_CHAN_ACCEL_XYZ, x) < 0 ||
        sensor_channel_get(bmi270_dev, SENSOR_CHAN_ACCEL_XYZ, y) < 0 ||
        sensor_channel_get(bmi270_dev, SENSOR_CHAN_ACCEL_XYZ, z) < 0) {
        LOG_ERR("Failed to get acceleration data");
        return -EIO;
    }

    return 0;
}

int bmi270_read_gyroscope(struct sensor_value *x, struct sensor_value *y, struct sensor_value *z)
{
    if (sensor_sample_fetch(bmi270_dev) < 0) {
        LOG_ERR("Failed to fetch gyroscope data");
        return -EIO;
    }

    if (sensor_channel_get(bmi270_dev, SENSOR_CHAN_GYRO_XYZ, x) < 0 ||
        sensor_channel_get(bmi270_dev, SENSOR_CHAN_GYRO_XYZ, y) < 0 ||
        sensor_channel_get(bmi270_dev, SENSOR_CHAN_GYRO_XYZ, z) < 0) {
        LOG_ERR("Failed to get gyroscope data");
        return -EIO;
    }

    return 0;
}
