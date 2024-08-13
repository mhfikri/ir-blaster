#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "dht.h"

#include "context.h"
#include "error.h"
#include "rmt_ac.h"
#include "temperature.h"

static const char *TAG = "temperature";

static void temperature_task(void *arg)
{
    context_t *context = (context_t *)arg;
    ARG_ERROR_CHECK(context != NULL, ERR_PARAM_NULL);

    esp_err_t err;
    float temperature, humidity;
    bool rmt_auto_on = true;
    bool rmt_auto_off = true;

    while (true) {
        err = dht_read_float_data(DHT_TYPE_AM2301, CONFIG_DHT22_DATA_GPIO, &humidity, &temperature);
        if (err == ESP_OK) {
            context_set_temp_humidity(context, temperature, humidity);
            if (context->rmt.auto_on.enable) {
                if (temperature >= context->rmt.auto_on.temp) {
                    if (rmt_auto_on) {
                        err = AC_Brand_Set(context->rmt.auto_on.rmt_id, true);
                        if (err == ESP_OK) {
                            rmt_ac_power_on();
                            rmt_auto_on = false;
                        }
                    }
                } else {
                    rmt_auto_on = true;
                }
            }
            if (context->rmt.auto_off.enable) {
                if (temperature <= context->rmt.auto_off.temp) {
                    if (rmt_auto_off) {
                        err = AC_Brand_Set(context->rmt.auto_off.rmt_id, true);
                        if (err == ESP_OK) {
                            rmt_ac_power_off();
                            rmt_auto_off = false;
                        }
                    }
                } else {
                    rmt_auto_off = true;
                }
            }
            ESP_LOGI(TAG, "Temperature: %.1fC Humidity: %.1f%%", temperature, humidity);
        } else {
            ESP_LOGE(TAG, "Cannot read data from DHT22, error 0x%X", err);
        }

        if (err == ESP_OK) {
            vTaskDelay(pdMS_TO_TICKS(30 * 1000));
        } else {
            vTaskDelay(pdMS_TO_TICKS(2 * 1000));
        }
    }
}

esp_err_t temperature_init(context_t *context)
{
    ARG_CHECK(context != NULL, ERR_PARAM_NULL);

    xTaskCreatePinnedToCore(temperature_task, "temperature", 2048, context, 10, NULL, tskNO_AFFINITY);
    return ESP_OK;
}
