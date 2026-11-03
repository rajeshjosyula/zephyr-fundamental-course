#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include "rajesh_driver.h"

LOG_MODULE_REGISTER(rajesh_driver_impl);

#define DT_DRV_COMPAT rajesh_driver

static const struct gpio_dt_spec my_led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

struct rajesh_driver_data {
	uint32_t counter;
};

static int rajesh_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	if (!gpio_is_ready_dt(&my_led)) {
		LOG_ERR("LED GPIO device not ready");
		return -ENODEV;
	}

	int res = gpio_pin_set_dt(&my_led, 1);
	if (res) {
		printk("GPIO Set failed with response: %d\n", res);
	}

	LOG_INF("LED turned ON");
	return 0;
}

static int rajesh_channel_get(const struct device *dev, enum sensor_channel chan,
			      struct sensor_value *val)
{
	if (!gpio_is_ready_dt(&my_led)) {
		LOG_ERR("LED GPIO device not ready");
		return -ENODEV;
	}

	int res = gpio_pin_set_dt(&my_led, 0);
	if (res) {
		printk("GPIO Clear failed with response: %d\n", res);
	}

	LOG_INF("LED turned OFF");
	return 0;
}

static DEVICE_API(sensor, rajesh_api) = {
	.sample_fetch = rajesh_sample_fetch,
	.channel_get = rajesh_channel_get,
};

static int rajesh_driver_init(const struct device *dev)
{
	LOG_INF("Rajesh Driver initializing...\n");

	if (!gpio_is_ready_dt(&my_led)) {
		LOG_ERR("LED GPIO device not ready");
		return -ENODEV;
	}

	int res = gpio_pin_configure_dt(&my_led, GPIO_OUTPUT_INACTIVE);
	if (res) {
		LOG_ERR("Failed to configure LED GPIO: %d", res);
		return res;
	}

	LOG_INF("Rajesh Driver initialized successfully");
	return 0;
}

uint32_t get_rajesh_data_counter(const struct device *dev)
{
	const struct rajesh_driver_data *data = dev->data;
	return data->counter;
}

int set_rajesh_data_counter(const struct device *dev, uint32_t value)
{
	struct rajesh_driver_data *data = dev->data;
	data->counter = value;
	return 0;
}

int increment_rajesh_data_counter(const struct device *dev)
{
	struct rajesh_driver_data *data = dev->data;
	data->counter += 1;
	return 0;
}

#define RAJESH_DRIVER_DEFINE(inst) \
	static struct rajesh_driver_data rajesh_driver_data_##inst; \
	DEVICE_DT_INST_DEFINE(inst, \
			      rajesh_driver_init, \
			      NULL, \
			      &rajesh_driver_data_##inst, \
			      NULL, \
			      POST_KERNEL, \
			      CONFIG_SENSOR_INIT_PRIORITY, \
			      &rajesh_api);

DT_INST_FOREACH_STATUS_OKAY(RAJESH_DRIVER_DEFINE)
