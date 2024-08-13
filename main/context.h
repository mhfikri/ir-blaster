#ifndef IR_BLASTER_CONTEXT_H
#define IR_BLASTER_CONTEXT_H

#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/portmacro.h"

#include "esp_bit_defs.h"

#define CONTEXT_UNKNOWN_VALUE INT16_MIN
#define CONTEXT_VALUE_IS_VALID(x) ((x) != CONTEXT_UNKNOWN_VALUE)

typedef enum {
    CONTEXT_EVENT_TEMPERATURE = BIT0,
    CONTEXT_EVENT_HUMIDITY = BIT1,
    CONTEXT_EVENT_WIFI = BIT2,
    CONTEXT_EVENT_NETWORK = BIT3,
    CONTEXT_EVENT_TIME = BIT4,
    CONTEXT_EVENT_IOT = BIT5,
    CONTEXT_EVENT_NETWORK_ERROR = BIT6,
    CONTEXT_EVENT_CONFIG = BIT7,
} context_event_t;

typedef struct {
    portMUX_TYPE spinlock;
    EventGroupHandle_t event_group;

    struct {
        const char *device_id;
        const char *ssid;
        const char *password;
    } config;

    struct {
        volatile float temp;
        volatile float humidity;
    } sensors;

    struct {
        struct {
            bool enable;
            int rmt_id;
            float temp;
        } auto_on;
        struct {
            bool enable;
            int rmt_id;
            float temp;
        } auto_off;
    } rmt;
} context_t;

context_t *context_create(void);

void context_lock(context_t *context);

void context_unlock(context_t *context);

esp_err_t context_set_temp_humidity(context_t *context, float temp, float humidity);

esp_err_t context_set_wifi_provisioned(context_t *context);

esp_err_t context_set_network_connected(context_t *context, bool connected);

esp_err_t context_set_network_error(context_t *context, bool error);

esp_err_t context_set_time_updated(context_t *context);

esp_err_t context_set_iot_connected(context_t *context, bool connected);

esp_err_t context_set_config(context_t *context, const char *device_id, const char *ssid, const char *password);

esp_err_t context_set_rmt_auto_on(context_t *context, bool enable,
                                  int rmt_id, float temp);

esp_err_t context_set_rmt_auto_off(context_t *context, bool enable,
                                   int rmt_id, float temp);

#endif // IR_BLASTER_CONTEXT_H
