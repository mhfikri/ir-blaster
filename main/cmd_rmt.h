#ifndef IR_BLASTER_DRIVER_H
#define IR_BLASTER_DRIVER_H

#define RMT_TYPE_AC 1
#define RMT_TYPE_TV 2

#define RMT_AC_ON 0
#define RMT_AC_OFF 1
#define RMT_AC_TEMP_UP 2
#define RMT_AC_TEMP_DOWN 3
#define RMT_AC_FAN_LOW 4
#define RMT_AC_FAN_MEDIUM 5
#define RMT_AC_FAN_HIGH 6
#define RMT_AC_SWING_ON 7
#define RMT_AC_SWING_OFF 8

#include "esp_err.h"

#include "cJSON.h"

esp_err_t cmd_rmt(cJSON *data);

#endif // IR_BLASTER_DRIVER_H
