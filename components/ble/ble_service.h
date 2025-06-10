#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void ble_service_init(void);
void ble_service_notify(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif // BLE_SERVICE_H
