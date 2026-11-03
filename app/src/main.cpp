#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <rajesh_driver.h>

static const struct device *const driver_dev = DEVICE_DT_GET(DT_NODELABEL(rajesh_driver0));

int main(void)
{
    if (!device_is_ready(driver_dev)) {
        return -ENODEV;
    }

    struct sensor_value value;

    int res = set_rajesh_data_counter(driver_dev, 100);
    if (res) {
        return res;
    }

    while (1) {
        res = sensor_sample_fetch(driver_dev);
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

        res = increment_rajesh_data_counter(driver_dev);
        if (res) {
            return res;
        }

        uint32_t counter_value = get_rajesh_data_counter(driver_dev);
        printk("The counter value in Rajesh Driver is %u\n", (unsigned int)counter_value);

    }

    return 0;
}
