#include "esp_event.h"
#include "esp_log.h"

#include "aws_iot.h"
#include "context.h"
#include "driver.h"
#include "ntp.h"
#include "storage.h"
#include "temperature.h"
#include "wifi.h"

static context_t *context;

void app_main(void)
{
    context = context_create();
    ESP_ERROR_CHECK(driver_init());
    ESP_ERROR_CHECK(storage_init(context));
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(wifi_init(context));
    ESP_ERROR_CHECK(ntp_init(context));
    ESP_ERROR_CHECK(aws_iot_init(context));
    // ESP_ERROR_CHECK(temperature_init(context));
}