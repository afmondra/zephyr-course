#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/**
 * @brief Custom extension API for our_driver.
 *
 * sensor MUST be the first member so that a pointer to this struct
 * is layout-compatible with struct sensor_driver_api *.
 * The Zephyr sensor subsystem casts dev->api to sensor_driver_api *
 * and finds sample_fetch / channel_get at the correct offset.
 * Application code casts dev->api back to our_driver_api * to reach
 * the custom extension functions below.
 */
struct our_driver_api {
    struct sensor_driver_api sensor;    /* standard sensor API — first! */
    int (*set_blink_interval_ms)(const struct device *dev, uint32_t ms);
};

/** Inline wrapper — this is what application code calls directly. */
static inline int our_driver_set_blink_interval_ms(const struct device *dev,
                                                    uint32_t ms)
{
    const struct our_driver_api *api =
        (const struct our_driver_api *)dev->api;
    return api->set_blink_interval_ms(dev, ms);
}

#endif /* OUR_DRIVER_H_ */
