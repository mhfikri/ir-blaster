#include <stdio.h>
#include <string.h>

#include "driver/periph_ctrl.h"
#include "driver/rmt.h"
#include "esp_err.h"
#include "esp_log.h"
#include "soc/rmt_reg.h"

#include "driver.h"
#include "rmt_ac.h"
#include "rmt_db_ac.h"

unsigned char *output_buf;
unsigned char current_temp, current_mode, current_fan, current_swing;
static unsigned char do_verify, do_verify2;
static unsigned char dType, dSize, dArray, db_data, mask, offset;

static const AC_DB_FORM *DbPtr;
static const WAVE_FORM *WavePtr;
static const DATA_FORM *DataPtr;

static void Verify_data();
static void IR_Generate();
static inline void Pulse_Gen(unsigned char pulse, rmt_item32_t *item);
static void Init_Output_Buf(unsigned char temp, AC_DATA_LST fan, AC_DATA_LST swing);

esp_err_t AC_Brand_Set(unsigned short Set_Num, bool is_auto)
{
    unsigned short DbAccessCode = 0;
    esp_err_t err = rmt_find_model(Set_Num,
                                   &AC_DbForm[0].Num,
                                   ac_db_size,
                                   sizeof(AC_DB_FORM) / 2,
                                   &DbAccessCode);
    if (err != ESP_OK) {
        return err;
    }
    DbPtr = &AC_DbForm[DbAccessCode];
    WavePtr = &AC_WaveForm[DbPtr->WaveForm];
    DataPtr = &AC_DataForm[DbPtr->DataForm];

    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = WavePtr->Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = WavePtr->Freq.Low * 10;
    if (is_auto) {
        Init_Output_Buf(0, FAN_AUTO, SWING_ON);
    } else {
        Init_Output_Buf(6, FAN_LOW, SWING_OFF);
    }
    return err;
}

static void Init_Output_Buf(unsigned char temp, AC_DATA_LST fan, AC_DATA_LST swing)
{
    unsigned char dSeq = 1, dArray;
    output_buf = calloc(DATA_MAP_SIZE, sizeof(unsigned char));
    do_verify = 0;
    current_temp = temp;

    while (dSeq < DATA_MAP_SIZE) {
        dArray = DataPtr->DataMap[dSeq].Array;
        switch (dArray) {
        case 2:
            output_buf[dArray] = DbPtr->Data[POWER_ON];
            break;
        case 3:
            output_buf[dArray] = DbPtr->Data[POWER_ON];
            break;
        case 4:
            output_buf[dArray] = DbPtr->Data[MODE_COOL];
            current_mode = output_buf[dArray];
            break;
        case 5:
            output_buf[dArray] = DbPtr->Data[swing];
            current_swing = output_buf[dArray];
            break;
        case 6:
            output_buf[dArray] = DbPtr->Data[fan];
            current_fan = output_buf[dArray];
            break;
        case 7:
            output_buf[dArray] = DbPtr->Temp[current_temp];
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            output_buf[dArray] = DbPtr->Default[dArray - 8];
            break;
        case 18:
            do_verify = 1;
            break;
        case 19:
            do_verify2 = 1;
            break;
        case 20:
            output_buf[dArray] = DbPtr->Keypress;
            break;
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            output_buf[dArray] = DbPtr->Default[dArray - 11];
            break;
        }
        dSeq++;
    }

    if (do_verify) {
        Verify_data();
    }
}

unsigned char reverse_bit(unsigned char num, int nbit)
{
    unsigned char reverse_num = 0;
    for (int i = 0; i < nbit; i++) {
        reverse_num <<= 1;
        reverse_num |= num & 1;
        num >>= 1;
    }
    return reverse_num;
}

unsigned char checksum_sum_byte(unsigned char start_data, unsigned char end_data, bool lsb)
{
    unsigned char cnt = 0;
    unsigned char csize = 0; // checksum size
    while (start_data < end_data) {
        dType = DataPtr->DataMap[start_data].Type;
        dArray = DataPtr->DataMap[start_data].Array;
        dSize = DataPtr->DataMap[start_data].Size;
        if (dType == DATA || dType == BAR) {
            if (dArray == 18 || dArray == 19) {
                csize = DataPtr->DataMap[start_data].Size;
            } else {
                db_data = output_buf[dArray];
                while (dSize < 8) {
                    offset = DataPtr->DataMap[start_data + 1].Size;
                    mask = DataPtr->DataMap[start_data + 1].Array;
                    db_data += (output_buf[mask] << dSize);
                    start_data++;
                    dSize += offset;
                }
                if (dType == BAR) {
                    cnt += lsb ? reverse_bit(~db_data, 8) : ~db_data;
                } else {
                    cnt += lsb ? reverse_bit(db_data, 8) : db_data;
                }
            }
        }
        start_data++;
    }
    return lsb ? reverse_bit(cnt, csize) : cnt;
}

unsigned char checksum_gree()
{
    unsigned char cnt = 0;
    unsigned char dSeq = 1;
    unsigned char byte_cnt = 1;
    while (dSeq < 8) {
        dType = DataPtr->DataMap[dSeq].Type;
        dArray = DataPtr->DataMap[dSeq].Array;
        dSize = DataPtr->DataMap[dSeq].Size;
        if (dType == DATA) {
            if (dArray != 18) {
                db_data = output_buf[dArray];
                while (dSize < 8) {
                    offset = DataPtr->DataMap[dSeq + 1].Size;
                    mask = DataPtr->DataMap[dSeq + 1].Array;
                    db_data += (output_buf[mask] << dSize);
                    dSeq++;
                    dSize += offset;
                }
                if (byte_cnt < 5) {
                    cnt += db_data & 0x0f;
                } else if (4 < byte_cnt && byte_cnt < 8) {
                    cnt += db_data >> 4;
                }
                byte_cnt++;
            }
        }
        dSeq++;
    }
    return 0xc + cnt;
}

void Verify_data()
{
    if (do_verify) {
        switch (DbPtr->Verify) {
        case PANASONIC152AC:
            output_buf[18] = checksum_sum_byte(0, DATA_MAP_SIZE, false);
            break;
        case SHARP67AC:
            output_buf[18] = checksum_gree();
            break;
        }
    }
}

static int rmt_build_items(rmt_item32_t *item)
{
    int i = 0;
    unsigned char dSeq = 0, dType, dSize, dPulse, dArray, db_data;
    unsigned char *output_ptr = output_buf;

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
            db_data = output_ptr[dArray];
            while (dSize) {
                if (db_data & 0x01)
                    Pulse_Gen(1, item);
                else
                    Pulse_Gen(0, item);
                dSize--;
                db_data >>= 1;
                item++;
                i++;
            }
            break;
        case BAR:
            db_data = output_ptr[dArray];
            while (dSize) {
                if (db_data & 0x01)
                    Pulse_Gen(0, item);
                else
                    Pulse_Gen(1, item);
                dSize--;
                db_data >>= 1;
                item++;
                i++;
            }
            break;
        }
        dSeq++;
    }
    return i;
}

static void IR_Generate()
{
    int item_num = 0;
    size_t size = (sizeof(rmt_item32_t) * RMT_DATA_ITEM_NUM);
    rmt_item32_t *item = (rmt_item32_t *)malloc(size);
    memset((void *)item, 0, size);
    item_num = rmt_build_items(item);
    // gpio_matrix_out(RMT_TX_EXTENDER_GPIO, RMT_TX_SIGNAL_IDX, false, false);
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

esp_err_t rmt_ac_power_on()
{
    if (DbPtr->Data[POWER_ON] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    output_buf[2] = DbPtr->Data[POWER_ON];
    output_buf[3] = DbPtr->Data[POWER_ON];

    switch (DbPtr->DataForm) {
    default:
        Verify_data();
        IR_Generate();
        break;
    }

    return ESP_OK;
}

esp_err_t rmt_ac_power_off()
{
    if (DbPtr->Data[POWER_OFF] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    output_buf[2] = DbPtr->Data[POWER_OFF];
    output_buf[3] = DbPtr->Data[POWER_OFF];

    switch (DbPtr->DataForm) {
    default:
        Verify_data();
        IR_Generate();
        break;
    }
    output_buf[3] = DbPtr->Data[POWER_ON];

    return ESP_OK;
}

esp_err_t rmt_ac_temp_up()
{
    if (DbPtr->Data[TEMP_UP] == 0xff) {
        if (current_temp < 12) {
            current_temp++;
        }
        if (DbPtr->Temp[current_temp] == 0xff) {
            return ESP_ERR_NOT_SUPPORTED;
        } else {
            output_buf[7] = DbPtr->Temp[current_temp];
        }
    } else {
        output_buf[2] = DbPtr->Data[TEMP_UP];
    }

    Verify_data();
    IR_Generate();

    return ESP_OK;
}

esp_err_t rmt_ac_temp_down()
{
    if (DbPtr->Data[TEMP_DOWN] == 0xff) {
        if (current_temp > 0) {
            current_temp--;
        }
        if (DbPtr->Temp[current_temp] == 0xff) {
            return ESP_ERR_NOT_SUPPORTED;
        } else {
            output_buf[7] = DbPtr->Temp[current_temp];
        }
    } else {
        output_buf[2] = DbPtr->Data[TEMP_DOWN];
    }

    Verify_data();
    IR_Generate();

    return ESP_OK;
}

esp_err_t rmt_ac_fan_auto()
{
    if (DbPtr->Data[FAN_AUTO] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    current_fan = DbPtr->Data[FAN_AUTO];
    output_buf[2] = DbPtr->Data[FAN_AUTO];
    output_buf[6] = DbPtr->Data[FAN_AUTO];

    Verify_data();
    IR_Generate();

    return ESP_OK;
}

esp_err_t rmt_ac_fan_low()
{
    if (DbPtr->Data[FAN_LOW] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    current_fan = DbPtr->Data[FAN_LOW];
    output_buf[2] = DbPtr->Data[FAN_LOW];
    output_buf[6] = DbPtr->Data[FAN_LOW];

    Verify_data();
    IR_Generate();

    return ESP_OK;
}

esp_err_t rmt_ac_fan_medium()
{
    if (DbPtr->Data[FAN_MEDIUM] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    current_fan = DbPtr->Data[FAN_MEDIUM];
    output_buf[2] = DbPtr->Data[FAN_MEDIUM];
    output_buf[6] = DbPtr->Data[FAN_MEDIUM];

    Verify_data();
    IR_Generate();

    return ESP_OK;
}

esp_err_t rmt_ac_fan_high()
{
    if (DbPtr->Data[FAN_HIGH] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    current_fan = DbPtr->Data[FAN_HIGH];
    output_buf[2] = DbPtr->Data[FAN_HIGH];
    output_buf[6] = DbPtr->Data[FAN_HIGH];

    Verify_data();
    IR_Generate();

    return ESP_OK;
}

esp_err_t rmt_ac_swing_on()
{
    if (DbPtr->Data[SWING_ON] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    current_swing = DbPtr->Data[SWING_ON];
    output_buf[2] = DbPtr->Data[SWING_ON];
    output_buf[5] = DbPtr->Data[SWING_ON];

    switch (DbPtr->DataForm) {
    case SHARP67AC_DM:
        output_buf[20] = 0x00;
        Verify_data();
        IR_Generate();
        break;

    default:
        Verify_data();
        IR_Generate();
        break;
    }

    return ESP_OK;
}

esp_err_t rmt_ac_swing_off()
{
    if (DbPtr->Data[SWING_OFF] == 0xff) {
        return ESP_ERR_NOT_SUPPORTED;
    }

    current_swing = DbPtr->Data[SWING_OFF];
    output_buf[2] = DbPtr->Data[SWING_OFF];
    output_buf[5] = DbPtr->Data[SWING_OFF];

    switch (DbPtr->DataForm) {
    case SHARP67AC_DM:
        output_buf[20] = 0x01;
        Verify_data();
        IR_Generate();
        break;

    default:
        Verify_data();
        IR_Generate();
        break;
    }

    return ESP_OK;
}
