#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>

#include "tmp117.h"

// Static handle to TMP117 sensor device
const struct device *tmp_dev = DEVICE_DT_GET(DT_NODELABEL(tmp117));

int tmp117_init(void)
{
    if (!device_is_ready(tmp_dev)) {
        printk("TMP117 device not ready\n");
        return -ENODEV;
    }

    printk("TMP117 initialized\n");
    return 0;
}

int tmp117_read_temperature(struct sensor_value *temp)
{
    if (sensor_sample_fetch(tmp_dev) < 0) {
        printk("TMP117 sample fetch failed\n");
        return -EIO;
    }

    if (sensor_channel_get(tmp_dev, SENSOR_CHAN_AMBIENT_TEMP, temp) < 0) {
        printk("TMP117 channel get failed\n");
        return -EIO;
    }

    return 0;
}
