#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 100

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

BUILD_ASSERT(DT_NODE_HAS_STATUS(LED_NODE, okay),
             "app-led alias is not defined or node is disabled");

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
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
                gpio_pin_set_dt(&led, 1);
                k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

                gpio_pin_set_dt(&led, 0);
                k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        }

        return 0;


/*     bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0; */
}
