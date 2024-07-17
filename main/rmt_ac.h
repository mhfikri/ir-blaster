#ifndef IR_BLASTER_RMT_AC_H
#define IR_BLASTER_RMT_AC_H

#include "esp_err.h"

typedef struct {
    unsigned char id;
    unsigned char state;
} CMD;

typedef enum {
    POWER_ON = 0,
    POWER_OFF,
    MODE_COOL,
    MODE_WARM,
    MODE_DRY,
    MODE_AUTO,
    FAN_LOW,
    FAN_MEDIUM,
    FAN_HIGH,
    FAN_AUTO,
    FAN_TURBO,
    SWING_ON = 27,
    SWING_OFF,
    TEMP_UP,
    TEMP_DOWN,
} AC_DATA_LST;

esp_err_t AC_Brand_Set(unsigned short Set_Num);
esp_err_t rmt_ac_power_on();
esp_err_t rmt_ac_power_off();
esp_err_t rmt_ac_temp_up();
esp_err_t rmt_ac_temp_down();
esp_err_t rmt_ac_fan_auto();
esp_err_t rmt_ac_fan_low();
esp_err_t rmt_ac_fan_medium();
esp_err_t rmt_ac_fan_high();
esp_err_t rmt_ac_swing_on();
esp_err_t rmt_ac_swing_off();

#endif // IR_BLASTER_RMT_AC_H