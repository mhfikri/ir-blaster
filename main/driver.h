#ifndef IR_BLASTER_DRIVER_H
#define IR_BLASTER_DRIVER_H

#include "esp_err.h"

#define RMT_TX_CHANNEL 0
#define RMT_TX_GPIO 27
#define RMT_TX_SIGNAL_IDX 87
#define RMT_TX_EXTENDER_GPIO 19
#define RMT_TICK_10_US 8
#define RMT_DATA_ITEM_NUM 256

esp_err_t driver_init(void);

esp_err_t driver_rmt_find_model(unsigned short Set_Num, unsigned short *setnum_ptr,
                                unsigned int db_list_size, unsigned char db_size,
                                unsigned short *DbAccessPtr);

#endif /* IR_BLASTER_DRIVER_H */
