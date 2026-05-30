#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// #define LED_SENSOR_NODE DT_NODELABEL(led_sensor0)

// BUILD_ASSERT(DT_NODE_HAS_STATUS(LED_SENSOR_NODE, okay),
// 	     "led_sensor0 node is not enabled");
#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

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

	while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;
        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
