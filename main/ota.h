#ifndef IR_BLASTER_OTA_H
#define IR_BLASTER_OTA_H

#include "esp_err.h"

#include "context.h"

const char *ota_get_app_version(void);

esp_err_t ota_init(char *url);

#endif // IR_BLASTER_OTA_H
