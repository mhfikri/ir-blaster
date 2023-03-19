#ifndef __AC_IR_TX_H__
#define __AC_IR_TX_H__

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
    FAN_MID,
    FAN_HIGH,
    FAN_AUTO,
    FAN_TURBO,
    SWING_ON = 27,
    SWING_OFF,
    TEMP_UP,
    TEMP_DOWN,
} AC_DATA_LST;

esp_err_t AC_Brand_Set(unsigned short Set_Num);

//air conditioner api list
esp_err_t ac_power_on();
esp_err_t ac_power_off();
esp_err_t ac_mode_cool();
esp_err_t ac_mode_warm();
esp_err_t ac_mode_dry();
esp_err_t ac_mode_auto();
esp_err_t ac_fan_low();
esp_err_t ac_fan_mid();
esp_err_t ac_fan_high();
esp_err_t ac_fan_auto();
esp_err_t ac_fan_turbo();
esp_err_t ac_swing_on();
esp_err_t ac_swing_off();
esp_err_t ac_temp_up();
esp_err_t ac_temp_down();

#endif
