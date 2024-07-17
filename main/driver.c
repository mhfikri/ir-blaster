#include "driver/gpio.h"
#include "driver/rmt.h"

#include "esp_err.h"
#include "esp_log.h"

#include "iot_button.h"
#include "led_indicator.h"

#include "driver.h"
#include "error.h"

static const char *TAG = "driver";

static led_indicator_handle_t led_handle = NULL;

typedef enum {
    BLINK_DOUBLE,
    BLINK_PROVISIONING,
    BLINK_PROVISIONED,
    BLINK_CONNECTING,
    BLINK_CONNECTED,
    BLINK_OTA,
    BLINK_NUM,
} led_blink_type_t;

static const blink_step_t double_blink[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 100},
    {LED_BLINK_HOLD, LED_STATE_OFF, 100},
    {LED_BLINK_HOLD, LED_STATE_ON, 100},
    {LED_BLINK_HOLD, LED_STATE_OFF, 100},
    {LED_BLINK_STOP, 0, 0},
};

static const blink_step_t led_provisioning[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 500},
    {LED_BLINK_HOLD, LED_STATE_OFF, 500},
    {LED_BLINK_LOOP, 0, 0},
};

static const blink_step_t led_provisioned[] = {
    {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
    {LED_BLINK_STOP, 0, 0},
};

static const blink_step_t led_connecting[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 200},
    {LED_BLINK_HOLD, LED_STATE_OFF, 800},
    {LED_BLINK_LOOP, 0, 0},
};

static const blink_step_t led_connected[] = {
    {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
    {LED_BLINK_STOP, 0, 0},
};

static const blink_step_t led_ota[] = {
    {LED_BLINK_HOLD, LED_STATE_ON, 200},
    {LED_BLINK_HOLD, LED_STATE_OFF, 200},
    {LED_BLINK_LOOP, 0, 0},
};

static blink_step_t const *led_blink_list[] = {
    [BLINK_DOUBLE] = double_blink,
    [BLINK_PROVISIONING] = led_provisioning,
    [BLINK_PROVISIONED] = led_provisioned,
    [BLINK_CONNECTING] = led_connecting,
    [BLINK_CONNECTED] = led_connected,
    [BLINK_OTA] = led_ota,
    [BLINK_NUM] = NULL,
};

static void button_single_click_cb(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BUTTON_SINGLE_CLICK");
}

static void button_long_press_start_cb(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "BUTTON_LONG_PRESS_START");
}

void led_double_blink(void)
{
    ESP_LOGI(TAG, "led double blink");
    led_indicator_start(led_handle, BLINK_DOUBLE);
}

void led_ota_blink_start(void)
{
    ESP_LOGI(TAG, "led ota blink start");
    led_indicator_start(led_handle, BLINK_OTA);
}

void led_ota_blink_stop(void)
{
    ESP_LOGI(TAG, "led ota blink stop");
    led_indicator_stop(led_handle, BLINK_OTA);
}

void led_provisioning_blink_start(void)
{
    ESP_LOGI(TAG, "led provisioning blink start");
    led_indicator_start(led_handle, BLINK_PROVISIONING);
}

void led_provisioning_blink_stop(void)
{
    ESP_LOGI(TAG, "led provisioning blink stop");
    led_indicator_stop(led_handle, BLINK_PROVISIONING);
}

static void rmt_tx_init(void)
{
    rmt_config_t rmt_cfg = RMT_DEFAULT_CONFIG_TX(RMT_TX_GPIO, RMT_TX_CHANNEL);
    rmt_cfg.tx_config.carrier_en = true;
    rmt_cfg.clk_div = 100;
    ESP_ERROR_CHECK(rmt_config(&rmt_cfg));
    ESP_ERROR_CHECK(rmt_driver_install(rmt_cfg.channel, 0, 0));
}

esp_err_t rmt_find_model(unsigned short Set_Num,
                         unsigned short *setnum_ptr,
                         unsigned int db_list_size,
                         unsigned char db_size,
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

esp_err_t driver_init(void)
{
    led_indicator_gpio_config_t led_gpio_cfg = {
        .gpio_num = LED_INDICATOR_GPIO,
        .is_active_level_high = true,
    };

    led_indicator_config_t led_cfg = {
        .led_indicator_gpio_config = &led_gpio_cfg,
        .mode = LED_GPIO_MODE,
        .blink_lists = led_blink_list,
        .blink_list_num = BLINK_NUM,
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

    rmt_tx_init();

    return ESP_OK;
}
