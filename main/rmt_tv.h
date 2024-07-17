#ifndef IR_BLASTER_RMT_TV_H
#define IR_BLASTER_RMT_TV_H

#include "esp_err.h"

typedef enum {
    RMT_TV_POWER = 0,
    RMT_TV_VOLUP,
    RMT_TV_VOLDN,
    RMT_TV_SOURCE,
    RMT_TV_UP,
    RMT_TV_DOWN,
    RMT_TV_LEFT,
    RMT_TV_RIGHT,
    RMT_TV_OK,
    RMT_TV_CHUP,
    RMT_TV_CHDN,
    RMT_TV_MENU,
    RMT_TV_MUTE,
    RMT_TV_RETURN,
    RMT_TV_HOME,
} TV_KEY_INDEX;

esp_err_t TV_IR_TX(unsigned char keycode);
esp_err_t TV_Brand_Set(unsigned short Set_Num);

#endif // IR_BLASTER_RMT_TV_H