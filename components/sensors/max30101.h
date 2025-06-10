#ifndef MAX30101_APP_INTERFACE_H_
#define MAX30101_APP_INTERFACE_H_

#include <zephyr/drivers/sensor.h>

int max30101_init(void);
int max30101_read_heart_rate(struct sensor_value *hr);
int max30101_read_spo2(struct sensor_value *spo2);
int max30101_sample_fetch(const struct device *dev);

#endif // MAX30101_APP_INTERFACE_H_
