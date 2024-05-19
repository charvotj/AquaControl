#include "relays_driver.h"

static const char *TAG = "mainBoard - relays_driver.c";
static i2c_dev_t pcf8574;
static uint8_t relays_state = 0;


esp_err_t relays_init()
{
    esp_err_t st = ESP_OK;
    ESP_LOGI(TAG,"relays_init() called");
    set_relays_module_status(MODST_STARTUP);
    st |= i2cdev_init();
    memset(&pcf8574, 0, sizeof(i2c_dev_t));
    st |= pcf8574_init_desc(&pcf8574, 0x23, 0, I2C_SDA_GPIO, I2C_SCL_GPIO);

    vTaskDelay (200 / portTICK_PERIOD_MS);
    st |= relays_set_all_force(0x00,true);
    vTaskDelay (2000 / portTICK_PERIOD_MS);
    st |= relays_set_all_force(0xff,true);
    vTaskDelay (2000 / portTICK_PERIOD_MS);
    st |= relays_set_all_force(0x00,true);

    if(ESP_OK == st)
    {
        set_relays_module_status(MODST_READY);
        ESP_LOGI(TAG,"relays_init() done, state READY");
    }
    else
    {
        set_relays_module_status(MODST_ERROR);
        ESP_LOGE(TAG,"relays_init() failed, state ERROR");
    }
    return st;
}

esp_err_t relays_get_all(uint8_t* relays_st)
{
    *relays_st = relays_state;

    return ESP_OK;
}
esp_err_t relays_set_all(uint8_t relays_st)
{
    return relays_set_all_force(relays_st, false);
}

esp_err_t relays_set_all_force(uint8_t relays_st, bool force)
{
    // mask bits
    if(!force)
    {
        // compile check for number of used relays
        #if RELAYS_NUM != 4u
            #error "Update this code please."
        #endif
        uint8_t mask = 0x00;
        mask |= (1 << RELAY_NUM_1);
        mask |= (1 << RELAY_NUM_2); 
        mask |= (1 << RELAY_NUM_3);
        mask |= (1 << RELAY_NUM_4);

        relays_st &= mask;
    }

    relays_state = relays_st;

    return pcf8574_port_write(&pcf8574, ~relays_state);
}


esp_err_t relays_get_state(relay_num_t relay_num, bool* relay_st)
{
    if(relay_num > 7)
        return ESP_FAIL;

    *relay_st = (bool)(0x01 & (relays_state >> relay_num));

    return ESP_OK;
}

esp_err_t relays_set_state(relay_num_t relay_num, bool relay_st)
{
    if(relay_num > 7)
        return ESP_FAIL;

    if(relay_st)
        relays_state |= (1 << relay_num);
    else
        relays_state &= ~(1 << relay_num);

    return pcf8574_port_write(&pcf8574, ~relays_state);
}

