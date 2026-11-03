#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>

static const struct device *const driver_dev = DEVICE_DT_GET(DT_NODELABEL(rajesh_driver0));

int main(void)
{
    if (!device_is_ready(driver_dev)) {
        return -ENODEV;
    }

    struct sensor_value value;

    while (1) {
        int res = sensor_sample_fetch(driver_dev);
        if (res) {
            printk("sensor_sample_fetch failed with response: %d\n", res);
            return res;
        }
        k_msleep(1000);

        res = sensor_channel_get(driver_dev, SENSOR_CHAN_ALL, &value);
        if (res) {
            printk("sensor_channel_get failed with response: %d\n", res);
            return res;
        }
        k_msleep(1000);
    }

    return 0;
}
