#include "driver/gpio.h"
#include "driver/rmt.h"

#include "esp_err.h"
#include "esp_log.h"

#include "iot_button.h"
#include "led_indicator.h"

#include "driver.h"
#include "error.h"

#define LED_INDICATOR_GPIO 32
#define BUTTON_GPIO 33
#define BUTTON_ACTIVE_LEVEL 1
#define RMT_TX_GPIO 17

static const char *TAG = "driver";

static led_indicator_handle_t led_handle = NULL;

static void button_single_click_cb(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BUTTON_SINGLE_CLICK");
}

static void button_long_press_start_cb(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BUTTON_LONG_PRESS_START");
}

esp_err_t driver_init(void)
{
    led_indicator_gpio_config_t led_gpio_cfg = {
        .gpio_num = LED_INDICATOR_GPIO,
        .is_active_level_high = true,
    };
    led_indicator_config_t led_cfg = {
        .mode = LED_GPIO_MODE,
        .led_indicator_gpio_config = &led_gpio_cfg,
    };
    led_handle = led_indicator_create(&led_cfg);
    if (led_handle == NULL) {
        ESP_LOGE(TAG, "led_indicator_create failed");
        return ESP_FAIL;
    }

    button_config_t btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = 3000,
        .short_press_time = 0,
        .gpio_button_config = {
            .gpio_num = BUTTON_GPIO,
            .active_level = BUTTON_ACTIVE_LEVEL,
        },
    };
    button_handle_t btn_handle = iot_button_create(&btn_cfg);
    if (btn_handle == NULL) {
        ESP_LOGE(TAG, "iot_button_create failed");
        return ESP_FAIL;
    }
    ESP_ERROR_CHECK(iot_button_register_cb(btn_handle, BUTTON_SINGLE_CLICK, button_single_click_cb, NULL));
    ESP_ERROR_CHECK(iot_button_register_cb(btn_handle, BUTTON_LONG_PRESS_START, button_long_press_start_cb, NULL));

    rmt_config_t rmt_cfg = RMT_DEFAULT_CONFIG_TX(RMT_TX_GPIO, RMT_TX_CHANNEL);
    rmt_cfg.tx_config.carrier_en = true;
    rmt_cfg.clk_div = 100;
    ESP_ERROR_CHECK(rmt_config(&rmt_cfg));
    ESP_ERROR_CHECK(rmt_driver_install(rmt_cfg.channel, 0, 0));

    return ESP_OK;
}

esp_err_t driver_rmt_find_model(unsigned short Set_Num, unsigned short *setnum_ptr,
                                unsigned int db_list_size, unsigned char db_size,
                                unsigned short *DbAccessPtr)
{
    for (int i = 0; i < db_list_size; i++) {
        if (*setnum_ptr == Set_Num) {
            *DbAccessPtr = i;
            return ESP_OK;
        }
        setnum_ptr += db_size;
    }
    return ESP_ERR_NOT_FOUND;
}
