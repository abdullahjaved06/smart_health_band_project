#ifndef ZEPHYR_DRIVERS_SENSOR_BMI270_H_
#define ZEPHYR_DRIVERS_SENSOR_BMI270_H_

#include <zephyr/drivers/sensor.h>

int bmi270_init(void);
int bmi270_read_acceleration(struct sensor_value *x, struct sensor_value *y, struct sensor_value *z);
int bmi270_read_gyroscope(struct sensor_value *x, struct sensor_value *y, struct sensor_value *z);

#endif /* ZEPHYR_DRIVERS_SENSOR_BMI270_H_ */
