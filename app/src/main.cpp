#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#define DT_DRV_COMPAT rajesh_driver

LOG_MODULE_REGISTER(rajesh_driver, CONFIG_SENSOR_LOG_LEVEL);

int main(void)
{
    const struct device* dev = DEVICE_DT_GET_ANY(rajesh_driver);

    if(!device_is_ready(dev)) {
        return -ENODEV;
    }

    struct sensor_value val;

    /* Test: Turn ON the LED using sensor_sample_fetch */
    int ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample: %d", ret);
        return ret;
    }

    k_msleep(1000);

    /* Test: Turn OFF the LED using sensor_channel_get */
    ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    if (ret) {
        LOG_ERR("Failed to get channel: %d", ret);
        return ret;
    }

    return 0;
}
