#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_https_ota.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_system.h"

#include "driver.h"
#include "error.h"
#include "ota.h"

static const char *TAG = "ota";

extern const uint8_t ota_cert_pem_start[] asm("_binary_ota_cert_pem_start");
extern const uint8_t ota_cert_pem_end[] asm("_binary_ota_cert_pem_end");

const char *ota_get_app_version(void)
{
    return esp_ota_get_app_description()->version;
}

static esp_err_t _http_client_init_cb(esp_http_client_handle_t http_client)
{
    return ESP_OK;
}

static void ota_task(void *arg)
{
    char *url = (char *)arg;
    ARG_ERROR_CHECK(url != NULL, ERR_PARAM_NULL);

    ESP_LOGI(TAG, "Starting OTA update...");

    esp_err_t ota_finish_err = ESP_OK;
    esp_http_client_config_t config = {
        .url = url,
        .cert_pem = (char *)ota_cert_pem_start,
        .timeout_ms = 5000,
        .keep_alive_enable = true,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
        .http_client_init_cb = _http_client_init_cb,
        .partial_http_download = true,
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_begin failed, error 0x%X", err);
        vTaskDelete(NULL);
    }

    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_get_img_desc failed, error 0x%X", err);
        goto ota_end;
    }

    while (1) {
        err = esp_https_ota_perform(https_ota_handle);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
            break;
        }
        // esp_https_ota_perform returns after every read operation which gives user the ability to
        // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
        // data read so far.
        ESP_LOGD(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
    }

    if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
        // the OTA image was not completely received and user can customise the response to this situation.
        ESP_LOGE(TAG, "Complete data was not received.");
    } else {
        led_ota_blink_stop();
        ota_finish_err = esp_https_ota_finish(https_ota_handle);
        if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
            ESP_LOGI(TAG, "OTA update successful, rebooting...");
            vTaskDelay(pdMS_TO_TICKS(1000));
            esp_restart();
        } else {
            if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
                ESP_LOGE(TAG, "Image validation failed, image is corrupted.");
            }
            ESP_LOGE(TAG, "esp_https_ota_finish failed, error 0x%X", ota_finish_err);
            vTaskDelete(NULL);
        }
    }

ota_end:
    esp_https_ota_abort(https_ota_handle);
    ESP_LOGE(TAG, "OTA update failed.");
    led_ota_blink_stop();
    vTaskDelete(NULL);
}

esp_err_t ota_init(char *url)
{
    ARG_CHECK(url != NULL, ERR_PARAM_NULL);

    led_ota_blink_start();
    xTaskCreate(ota_task, "ota_task", 8192, url, 7, NULL);
    return ESP_OK;
}
