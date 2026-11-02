#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(rajesh_driver_impl);

#define DT_DRV_COMPAT rajesh_driver

struct rajesh_driver_data {
	const struct device *gpio_dev;
	gpio_pin_t led_pin;
};

static int rajesh_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	struct rajesh_driver_data *data = dev->data;

	if (!device_is_ready(data->gpio_dev)) {
		LOG_ERR("GPIO device not ready");
		return -ENODEV;
	}

	/* Turn ON the LED */
	int ret = gpio_pin_set(data->gpio_dev, data->led_pin, 1);
	if (ret) {
		LOG_ERR("Failed to set GPIO pin: %d", ret);
		return ret;
	}

	LOG_INF("LED turned ON");
	return 0;
}

static int rajesh_channel_get(const struct device *dev, enum sensor_channel chan,
			      struct sensor_value *val)
{
	struct rajesh_driver_data *data = dev->data;

	if (!device_is_ready(data->gpio_dev)) {
		LOG_ERR("GPIO device not ready");
		return -ENODEV;
	}

	/* Turn OFF the LED */
	int ret = gpio_pin_set(data->gpio_dev, data->led_pin, 0);
	if (ret) {
		LOG_ERR("Failed to clear GPIO pin: %d", ret);
		return ret;
	}

	/* Store the state in the sensor value (0 = off) */
	val->val1 = 0;
	val->val2 = 0;

	LOG_INF("LED turned OFF");
	return 0;
}

static const struct sensor_driver_api rajesh_api = {
	.sample_fetch = rajesh_sample_fetch,
	.channel_get = rajesh_channel_get,
};

static int rajesh_driver_init(const struct device *dev)
{
	struct rajesh_driver_data *data = dev->data;

	data->gpio_dev = DEVICE_DT_GET(DT_ALIAS(led0));
	if (!device_is_ready(data->gpio_dev)) {
		LOG_ERR("LED device not ready");
		return -ENODEV;
	}

	data->led_pin = DT_GPIO_PIN(DT_ALIAS(led0), gpios);

	/* Configure the GPIO pin as output */
	int ret = gpio_pin_configure(data->gpio_dev, data->led_pin, GPIO_OUTPUT_INACTIVE);
	if (ret) {
		LOG_ERR("Failed to configure GPIO: %d", ret);
		return ret;
	}

	LOG_INF("Rajesh Driver initialized successfully");
	return 0;
}

static struct rajesh_driver_data rajesh_driver_data;

DEVICE_DT_INST_DEFINE(0,
		      rajesh_driver_init,
		      NULL,
		      &rajesh_driver_data,
		      NULL,
		      POST_KERNEL,
		      CONFIG_SENSOR_INIT_PRIORITY,
		      &rajesh_api);
