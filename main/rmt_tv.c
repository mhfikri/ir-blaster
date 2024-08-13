#include <stdio.h>
#include <string.h>

#include "driver/periph_ctrl.h"
#include "driver/rmt.h"
#include "esp_err.h"
#include "esp_log.h"
#include "soc/rmt_reg.h"

#include "driver.h"
#include "rmt_db_tv.h"
#include "rmt_tv.h"

static const char *TAG = "rmt_tv";

static TV_DB_FORM *DbPtr;
static WAVE_FORM *WavePtr;
static DATA_FORM *DataPtr;

static unsigned char data[8];
static bool flag;

static void IR_Generate(unsigned char keycode);
static inline void Pulse_Gen(unsigned char pulse, rmt_item32_t *item);

esp_err_t TV_Brand_Set(unsigned short Set_Num)
{
    unsigned short DbAccessCode = 0;
    esp_err_t err = rmt_find_model(Set_Num,
                                   &TV_DbForm[0].Num,
                                   tv_db_size,
                                   sizeof(TV_DB_FORM) / 2,
                                   &DbAccessCode);
    if (err != ESP_OK) {
        return err;
    }
    DbPtr = &TV_DbForm[DbAccessCode];
    WavePtr = &TV_WaveForm[DbPtr->WaveForm];
    DataPtr = &TV_DataForm[DbPtr->DataForm];

    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = WavePtr->Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = WavePtr->Freq.Low * 10;
    return err;
}

esp_err_t TV_IR_TX(unsigned char keycode)
{
    if (DbPtr->Key[keycode] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }
    IR_Generate(keycode);
    return ESP_OK;
}

static int rmt_build_items(rmt_item32_t *item, unsigned char keycode)
{
    int i = 0;
    unsigned char dSeq = 0, dType, dSize, dPulse, dArray, db_data;
    data[0] = DbPtr->Custom1;
    data[1] = DbPtr->Custom2;
    data[2] = DbPtr->Key[keycode];
    data[3] = DbPtr->Default[0];
    data[4] = DbPtr->Default[1];
    data[5] = DbPtr->Default[2];
    data[6] = DbPtr->Default[3];

    while (dSeq < DATA_MAP_SIZE) {
        dType = DataPtr->DataMap[dSeq].Type;
        dSize = DataPtr->DataMap[dSeq].Size;
        dPulse = DataPtr->DataMap[dSeq].Pulse;
        dArray = DataPtr->DataMap[dSeq].Array;

        switch (dType) {
        case LEADER:
        case END:
            Pulse_Gen(dPulse, item++);
            i++;
            break;

        case DATA:
            db_data = data[dArray];
            while (dSize) {
                if (db_data & 0x01) {
                    Pulse_Gen(1, item);
                } else {
                    Pulse_Gen(0, item);
                }
                db_data >>= 1;
                dSize--;
                item++;
                i++;
            }
            break;

        case BAR:
            db_data = data[dArray];
            while (dSize) {
                if (db_data & 0x01) {
                    Pulse_Gen(0, item);
                } else {
                    Pulse_Gen(1, item);
                }
                db_data >>= 1;
                dSize--;
                item++;
                i++;
            }
            break;

        case TOGGLE:
            db_data = flag ? data[dArray] : ~data[dArray];
            flag = !flag;
            while (dSize) {
                if (db_data & 0x01) {
                    Pulse_Gen(1, item);
                } else {
                    Pulse_Gen(0, item);
                }
                db_data >>= 1;
                dSize--;
                item++;
                i++;
            }
            break;
        }
        dSeq++;
    }
    return i;
}

static void IR_Generate(unsigned char keycode)
{
    int item_num = 0;
    size_t size = (sizeof(rmt_item32_t) * RMT_DATA_ITEM_NUM);
    rmt_item32_t *item = (rmt_item32_t *)malloc(size);
    memset((void *)item, 0, size);
    item_num = rmt_build_items(item, keycode);
    gpio_matrix_out(RMT_TX_EXTENDER_GPIO, RMT_TX_SIGNAL_IDX, false, false);
    rmt_write_items(RMT_TX_CHANNEL, item, item_num, true);
    led_double_blink();
    free(item);
}

static inline void Pulse_Gen(unsigned char pulse, rmt_item32_t *item)
{
    unsigned char seq = 0;
    unsigned short *time;
    unsigned char *pol;

    time = &(WavePtr->Pulse[pulse][seq].Time);
    pol = &(WavePtr->Pulse[pulse][seq].Pol);

    item->level0 = (*pol) - 1;
    item->duration0 = (*time) / 10 * RMT_TICK_10_US;
    pol += 4;
    time += 2;
    item->level1 = (*pol) - 1;
    item->duration1 = (*time) / 10 * RMT_TICK_10_US;
}
