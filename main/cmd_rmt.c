#include "esp_err.h"
#include "esp_log.h"

#include "cJSON.h"

#include "cmd_rmt.h"
#include "rmt_ac.h"

static const char *TAG = "cmd_rmt";

static int current_model = -1;

esp_err_t cmd_rmt(cJSON *data)
{
    esp_err_t err = ESP_OK;
    int device_type = cJSON_GetObjectItem(data, "deviceType")->valueint;
    int model_id = cJSON_GetObjectItem(data, "modelId")->valueint;
    int rmt_code = cJSON_GetObjectItem(data, "rmtCode")->valueint;

    switch (device_type) {
    case RMT_TYPE_AC:
        ESP_LOGI(TAG, "RMT_TYPE_AC");
        if (model_id != current_model) {
            err = AC_Brand_Set(model_id);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Cannot find model id %d", model_id);
                return err;
            }
            current_model = model_id;
        }

        switch (rmt_code) {
        case RMT_AC_ON:
            err = rmt_ac_power_on();
            break;
        case RMT_AC_OFF:
            err = rmt_ac_power_off();
            break;
        case RMT_AC_TEMP_UP:
            err = rmt_ac_temp_up();
            break;
        case RMT_AC_TEMP_DOWN:
            err = rmt_ac_temp_down();
            break;
        case RMT_AC_FAN_LOW:
            err = rmt_ac_fan_low();
            break;
        case RMT_AC_FAN_MEDIUM:
            err = rmt_ac_fan_medium();
            break;
        case RMT_AC_FAN_HIGH:
            err = rmt_ac_fan_high();
            break;
        case RMT_AC_SWING_ON:
            err = rmt_ac_swing_on();
            break;
        case RMT_AC_SWING_OFF:
            err = rmt_ac_swing_off();
            break;
        default:
            ESP_LOGE(TAG, "Invalid command code: %d", rmt_code);
            err = ESP_ERR_INVALID_ARG;
        }
        break;
    case RMT_TYPE_TV:
        ESP_LOGI(TAG, "RMT_TYPE_TV");
        break;
    default:
        ESP_LOGE(TAG, "Invalid device type: %d", device_type);
    }

    return err;
}
