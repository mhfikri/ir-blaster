#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_https_ota.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_system.h"

#include "error.h"
#include "ota.h"

const char *ota_get_app_version(void)
{
    return esp_ota_get_app_description()->version;
}
