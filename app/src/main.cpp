#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "/Users/bytelight/zephyr-course/app/drivers/our_driver/our_driver.h"

extern const __attribute__((__aligned__(__alignof(
    struct log_source_const_data)))) struct log_source_const_data log_const_main
    __attribute__((section("."
                           "_log_const"
                           "."
                           "static"
                           "."
                           "log_const_main_")))
    __attribute__((__used__)) = {.name = "main", .level = (3)};
extern const struct log_source_const_data log_const_main;
extern struct log_source_dynamic_data log_dynamic_main;
static const struct log_source_const_data *__log_current_const_data
    __attribute__((__unused__)) = 1 ? &log_const_main : __null;
static struct log_source_dynamic_data *__log_current_dynamic_data
    __attribute__((__unused__)) = (1 && 0) ? &log_dynamic_main : __null;
static const uint32_t __log_level __attribute__((__unused__)) = 3;

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
struct sensor_value val;

namespace {
	void test() {
		const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
		struct sensor_value val;
		auto ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
		LOG_INF("Channel ret %d", ret);
	}
}

int main(void)
{
	test();
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

	int ret;

	if (!gpio_is_ready_dt(&led)) {
			printk("Error: LED GPIO device not ready\n");
			return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
			printk("Error %d: failed to configure LED GPIO\n", ret);
			return ret;
	}

	printk("Heartbeat LED started — period: %d ms\n",
			CONFIG_APP_HEARTBEAT_PERIOD_MS);

	// --- Custom extension API call: change blink_interval_ms in the data struct ---
	our_driver_set_blink_interval_ms(driver, 250);

	while (1) {
		// Turns LED ON
		sensor_sample_fetch(driver);
		k_msleep(1000);
		// Turns LED OFF
		sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
		k_msleep(1000);
	}

	return 0;
}
