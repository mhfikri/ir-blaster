#ifndef IR_BLASTER_DRIVER_H
#define IR_BLASTER_DRIVER_H

#include "esp_err.h"

#define LED_INDICATOR_GPIO 22

#define BUTTON_GPIO 33
#define BUTTON_ACTIVE_LEVEL 1

#define RMT_TX_GPIO 16
#define RMT_TX_CHANNEL 0
#define RMT_TICK_10_US 8
#define RMT_DATA_ITEM_NUM 256
#define RMT_TX_EXTENDER_GPIO 19
#define RMT_TX_SIGNAL_IDX 87

void led_double_blink(void);
void led_ota_blink_start(void);
void led_ota_blink_stop(void);
void led_provisioning_blink_start(void);
void led_provisioning_blink_stop(void);
void led_connecting_blink_start(void);
void led_connecting_blink_stop(void);

esp_err_t rmt_find_model(unsigned short Set_Num,
                         unsigned short *setnum_ptr,
                         unsigned int db_list_size,
                         unsigned char db_size,
                         unsigned short *DbAccessPtr);

esp_err_t driver_init(void);

#endif // IR_BLASTER_DRIVER_H
