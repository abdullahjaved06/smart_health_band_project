#ifndef TMP117_APP_INTERFACE_H_
#define TMP117_APP_INTERFACE_H_

#include <zephyr/drivers/sensor.h>

int tmp117_init(void);
int tmp117_read_temperature(struct sensor_value *temp);

#endif // TMP117_APP_INTERFACE_H_
