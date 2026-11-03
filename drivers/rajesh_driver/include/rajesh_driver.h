#pragma once

#include <stdint.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t get_rajesh_data_counter(const struct device *dev);
int set_rajesh_data_counter(const struct device *dev, uint32_t value);
int increment_rajesh_data_counter(const struct device *dev);

#ifdef __cplusplus
}
#endif
