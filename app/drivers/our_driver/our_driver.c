#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

// --- Config struct: holds the GPIO pin from DT ---
struct our_driver_config {
    struct gpio_dt_spec led;
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

static DEVICE_API(sensor, api_iomico_lecture) = {
    .sample_fetch = sample_fetch_impl,
    .channel_get = channel_get_impl,
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

// --- Config instance: pull gpios from DT ---
static const struct our_driver_config our_driver_cfg_0 = {
    .led = GPIO_DT_SPEC_INST_GET(0, gpios),
};

/* #define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST); */

DEVICE_DT_INST_DEFINE(0, init, NULL,
                      NULL, &our_driver_cfg_0,
                      POST_KERNEL, 80, &api_iomico_lecture);
                      