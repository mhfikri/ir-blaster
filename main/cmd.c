#include <stdio.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "cJSON.h"

#include "cmd.h"
#include "driver.h"
#include "ota.h"
#include "rmt_ac.h"
#include "rmt_tv.h"

#define RMT_TYPE_AC 1
#define RMT_TYPE_TV 2

#define RMT_AC_ON 0
#define RMT_AC_OFF 1
#define RMT_AC_TEMP_UP 2
#define RMT_AC_TEMP_DOWN 3
#define RMT_AC_FAN_AUTO 4
#define RMT_AC_FAN_LOW 5
#define RMT_AC_FAN_MEDIUM 6
#define RMT_AC_FAN_HIGH 7
#define RMT_AC_SWING_ON 8
#define RMT_AC_SWING_OFF 9

static const char *TAG = "cmd";

static int prev_rmt_id = -1;

static esp_err_t cmd_rmt(cJSON *data)
{
    esp_err_t err = ESP_FAIL;

    int rmt_type = cJSON_GetObjectItem(data, "remoteType")->valueint;
    int rmt_id = cJSON_GetObjectItem(data, "remoteId")->valueint;
    int rmt_code = cJSON_GetObjectItem(data, "remoteCode")->valueint;

    if (rmt_type == RMT_TYPE_AC) {
        if (rmt_id != prev_rmt_id) {
            err = AC_Brand_Set(rmt_id);
            if (err != ESP_OK) {
                return err;
            }
            prev_rmt_id = rmt_id;
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
        case RMT_AC_FAN_AUTO:
            err = rmt_ac_fan_auto();
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
            ESP_LOGE(TAG, "Invalid remote code: %d", rmt_code);
            err = ESP_ERR_INVALID_ARG;
            break;
        }
    } else if (rmt_type == RMT_TYPE_TV) {
        err = TV_Brand_Set(rmt_id);
        if (err != ESP_OK) {
            return err;
        }
        err = TV_IR_TX(rmt_code);
    }

    if (err == ESP_OK) {
        led_double_blink();
    }

    return err;
}

void handle_command(char *cmd)
{
    cJSON *root = cJSON_Parse(cmd);
    int type = cJSON_GetObjectItem(root, "commandType")->valueint;
    cJSON *payload = cJSON_GetObjectItem(root, "commandPayload");

    if (type == CMD_TYPE_RESET) {
        ESP_LOGI(TAG, "Resetting in 3 seconds");
        vTaskDelay(pdMS_TO_TICKS(3000));
        nvs_flash_erase();
        esp_restart();
    } else if (type == CMD_TYPE_OTA) {
        cJSON *bin = cJSON_GetObjectItem(payload, "binaryUrl");
        char *url = malloc(strlen(bin->valuestring) + 1);
        strcpy(url, bin->valuestring);
        ota_init(url);
    } else if (type == CMD_TYPE_RMT) {
        cmd_rmt(payload);
    }
}