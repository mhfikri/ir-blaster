#include "driver/gpio.h"
#include "driver/rmt.h"

#include "esp_err.h"
#include "esp_log.h"

#include "driver.h"

esp_err_t driver_init(void)
{
    rmt_config_t rmt_cfg = RMT_DEFAULT_CONFIG_TX(RMT_TX_GPIO, RMT_TX_CHANNEL);
    rmt_cfg.tx_config.carrier_en = true;
    rmt_cfg.clk_div = 100;
    ESP_ERROR_CHECK(rmt_config(&rmt_cfg));
    ESP_ERROR_CHECK(rmt_driver_install(rmt_cfg.channel, 0, 0));

    return ESP_OK;
}

esp_err_t driver_rmt_find_model(unsigned short Set_Num, unsigned short *setnum_ptr,
                                unsigned int db_list_size, unsigned char db_size,
                                unsigned short *DbAccessPtr)
{
    for (int i = 0; i < db_list_size; i++) {
        if (*setnum_ptr == Set_Num) {
            *DbAccessPtr = i;
            return ESP_OK;
        }
        setnum_ptr += db_size;
    }
    return ESP_ERR_NOT_FOUND;
}