#include "CAN_driver.h"

static const char *TAG = "mainBoard - CAN_driver.c";

/* --------------------- Definitions and static variables ------------------ */
static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_125KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_GPIO, CAN_RX_GPIO, TWAI_MODE_NORMAL);


esp_err_t can_driver_init()
{
    esp_err_t status = gpio_set_direction(CAN_STBY_GPIO,GPIO_MODE_OUTPUT);

    status |= twai_driver_install(&g_config, &t_config, &f_config);
    ESP_LOGI(TAG, "Driver installed");
    status |= twai_start();
    ESP_LOGI(TAG, "CAN Driver started");
    status |= gpio_set_level(CAN_STBY_GPIO,0);

    return status;
}

esp_err_t can_proccess_rx(TickType_t timeout)
{
    //Listen for debug print from slave
    twai_message_t rx_msg;
    esp_err_t status = twai_receive(&rx_msg, timeout);
    if(status != ESP_OK)
        return status;

    if ((rx_msg.identifier & CAN_CMD_TYPE_MASK) == CAN_DEBUG_PRINT) {
        can_process_cmd_debug_print(&rx_msg);
    }
    else if ((rx_msg.identifier & CAN_CMD_TYPE_MASK) == CAN_TO_MASTER) {
        can_process_cmd_to_master(&rx_msg);
    }
    return status;
}

esp_err_t can_process_cmd_debug_print(twai_message_t *message)
{
    #ifdef CAN_DEBUG_PRINTS
    uint8_t slave_address = CAN_SID_to_adress(message->identifier);
    printf("CAN dbg from %d; SID: 0x%04lX MSG: ",slave_address, message->identifier);

    for (uint8_t i = 0; i < message->data_length_code; i++) {
        printf("%c", (char)message->data[i]);
    }
    printf("\n");    
    #endif
    return ESP_OK;
}

esp_err_t can_process_cmd_to_master(twai_message_t *message)
{
    uint8_t slave_address = CAN_SID_to_adress(message->identifier);
    printf("CAN TM; slave: 0x%02X\n",slave_address);
    return ESP_OK;
}

esp_err_t can_driver_deinit()
{
    esp_err_t status = twai_stop();
    ESP_LOGI(TAG, "Driver stopped");
    status |= twai_driver_uninstall();
    ESP_LOGI(TAG, "Driver uninstalled");
    return status;
}
