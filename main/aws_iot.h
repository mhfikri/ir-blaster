#ifndef IR_BLASTER_AWS_IOT_H
#define IR_BLASTER_AWS_IOT_H

#include "esp_err.h"

#include "context.h"

#define CONFIG_AWS_EXAMPLE_CLIENT_ID "IRB_03F8C8"

esp_err_t aws_iot_init(context_t *context);

#endif // IR_BLASTER_AWS_IOT_H