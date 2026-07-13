#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "our_driver.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

// --- Config struct: holds the GPIO pin from DT (read-only after init) ---
struct our_driver_config {
    struct gpio_dt_spec led;
};

// --- Data struct: mutable state changed at runtime ---
struct our_driver_data {
    uint32_t blink_interval_ms;
};

// --- sample_fetch: turn the LED ON ---
static int sample_fetch_impl(const struct device *dev,
                              enum sensor_channel chan)
{
    const struct our_driver_config *cfg = dev->config;
    LOG_INF("sample_fetch: LED ON");
    return gpio_pin_set_dt(&cfg->led, 1);
}

// --- channel_get: turn the LED OFF ---
static int channel_get_impl(const struct device *dev,
                             enum sensor_channel chan,
                             struct sensor_value *val)
{
    const struct our_driver_config *cfg = dev->config;
    LOG_INF("channel_get: LED OFF, channel %d", chan);
    val->val1 = 0;
    val->val2 = 0;
    return gpio_pin_set_dt(&cfg->led, 0);
}

static int channel_get_my_impl(  const struct device *dev, 
                                enum sensor_channel chan, 
                                struct sensor_value *val)
{
    LOG_INF("Hello from Channel Get, channel %d", chan);
    return 0;
};

// --- Custom extension: update blink_interval_ms in the data struct ---
static int set_blink_interval_ms_impl(const struct device *dev, uint32_t ms)
{
    struct our_driver_data *data = dev->data;
    LOG_INF("blink_interval_ms: %u -> %u", data->blink_interval_ms, ms);
    data->blink_interval_ms = ms;
    return 0;
}

// --- Extended API: sensor MUST be first so layout matches sensor_driver_api * ---
static const struct our_driver_api api_iomico_lecture = {
    .sensor = {
        .sample_fetch = sample_fetch_impl,
        .channel_get  = channel_get_impl,
    },
    .set_blink_interval_ms = set_blink_interval_ms_impl,
};

/* //Init fn
static int init(const struct device *dev)
{
    ARG_UNUSED(dev);

    LOG_INF("Device Initialized");

    return 0;
}
 */
// --- Init: configure the GPIO pin as output ---
static int init(const struct device *dev)
{
    const struct our_driver_config *cfg = dev->config;

    if (!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }

    LOG_INF("Device Initialized");
    return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

// --- Config instance (read-only): pulled from DT at build time ---
static const struct our_driver_config our_driver_cfg_0 = {
    .led = GPIO_DT_SPEC_INST_GET(0, gpios),
};

// --- Data instance (mutable): live state modified at runtime ---
static struct our_driver_data our_driver_data_0 = {
    .blink_interval_ms = 500,   /* default value */
};

/* #define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST); */

DEVICE_DT_INST_DEFINE(0, init, NULL,
                      &our_driver_data_0, &our_driver_cfg_0,
                      POST_KERNEL, 80, &api_iomico_lecture);
                      