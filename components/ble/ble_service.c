#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>

#include "ble_service.h"

LOG_MODULE_REGISTER(ble_custom, LOG_LEVEL_INF);

/* UUIDs */
#define BT_UUID_CUSTOM_SERVICE_VAL \
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)

#define BT_UUID_SENSOR_CHAR_VAL \
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)

#define BT_UUID_SLEEP_CHAR_VAL \
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef2)

static struct bt_uuid_128 custom_service_uuid = BT_UUID_INIT_128(BT_UUID_CUSTOM_SERVICE_VAL);
static struct bt_uuid_128 sensor_char_uuid    = BT_UUID_INIT_128(BT_UUID_SENSOR_CHAR_VAL);
static struct bt_uuid_128 sleep_char_uuid     = BT_UUID_INIT_128(BT_UUID_SLEEP_CHAR_VAL);

static uint8_t sensor_value = 0;
static uint16_t sleep_time = 1;
static struct bt_conn *current_conn = NULL;
static bool notify_enabled = false;

/* CCCD Handler */
static void ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	notify_enabled = (value == BT_GATT_CCC_NOTIFY);
	LOG_INF("Notifications %s", notify_enabled ? "enabled" : "disabled");
}

/* Read Sensor Value */
static ssize_t read_sensor_value(struct bt_conn *conn,
				 const struct bt_gatt_attr *attr, void *buf,
				 uint16_t len, uint16_t offset)
{
	LOG_INF("Sensor value read: %d", sensor_value);
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
				 &sensor_value, sizeof(sensor_value));
}

/* Write Sleep Time */
static ssize_t write_sleep_time(struct bt_conn *conn,
                                const struct bt_gatt_attr *attr,
                                const void *buf, uint16_t len,
                                uint16_t offset, uint8_t flags)
{
	LOG_WRN("Write Sleep Time triggered, len=%u", len);

	if (len == 1) {
		sleep_time = *((uint8_t *)buf);
	} else if (len == 2) {
		sleep_time = sys_get_le16(buf);
	} else {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	LOG_INF("Sleep time set to: %u minutes", sleep_time);
	return len;
}

/* GATT Service Definition */
BT_GATT_SERVICE_DEFINE(custom_svc,
	BT_GATT_PRIMARY_SERVICE(&custom_service_uuid),
	BT_GATT_CHARACTERISTIC(&sensor_char_uuid.uuid,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_sensor_value, NULL, &sensor_value),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(&sleep_char_uuid.uuid,
			       BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE,
			       NULL, write_sleep_time, &sleep_time),
);

/* Connection Callbacks */
static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("Connection failed (err %u)", err);
		return;
	}
	LOG_INF("Connected");
	current_conn = conn;
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason %u)", reason);
	current_conn = NULL;
	notify_enabled = false;
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

/* BLE Init */
void ble_service_init(void)
{
	int err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return;
	}

	LOG_INF("Bluetooth initialized");

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, NULL, 0, NULL, 0);
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
	} else {
		LOG_INF("Advertising started");
	}
}

/* BLE Notify */
void ble_service_notify(const uint8_t *data, size_t len)
{
	if (notify_enabled && current_conn) {
		int err = bt_gatt_notify(current_conn, &custom_svc.attrs[1], data, len);
		if (err) {
			LOG_ERR("Notification failed (err %d)", err);
		} else {
			LOG_INF("Notification sent");
		}
	}
}
