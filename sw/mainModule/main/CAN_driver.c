#include "CAN_driver.h"

static const char *TAG = "mainBoard - CAN_driver.c";

/* --------------------- Definitions and static variables ------------------ */
static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_125KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_GPIO, CAN_RX_GPIO, TWAI_MODE_NORMAL);

#define CAN_RX_TM_QUEUE_LEN 10
static QueueHandle_t can_rx_to_master_queue;
static SemaphoreHandle_t can_tx_mutex;
SemaphoreHandle_t can_rx_sem;


esp_err_t can_driver_init()
{
    can_rx_to_master_queue = xQueueCreate(CAN_RX_TM_QUEUE_LEN,sizeof(twai_message_t*));
    can_rx_sem = xSemaphoreCreateBinary();
    can_tx_mutex = xSemaphoreCreateMutex();

    esp_err_t status = gpio_set_direction(CAN_STBY_GPIO,GPIO_MODE_OUTPUT);

    status |= twai_driver_install(&g_config, &t_config, &f_config);
    ESP_LOGI(TAG, "Driver installed");
    status |= twai_start();
    ESP_LOGI(TAG, "CAN Driver started");
    status |= gpio_set_level(CAN_STBY_GPIO,0);

    // enable for tx
    xSemaphoreGive(can_tx_mutex);

    return status;
}



esp_err_t can_proccess_rx()
{
    twai_message_t rx_msg;
    while (ESP_OK == twai_receive(&rx_msg, 0))
    {
        if ((rx_msg.identifier & CAN_CMD_TYPE_MASK) == CAN_DEBUG_PRINT) {
            can_process_cmd_debug_print(&rx_msg);
        }
        else if ((rx_msg.identifier & CAN_CMD_TYPE_MASK) == CAN_TO_MASTER) {
            // Allocate memory for the structure to persist after function exit
            twai_message_t* ptr_msg = (twai_message_t *)pvPortMalloc(sizeof(twai_message_t));
            memcpy(ptr_msg, &rx_msg, sizeof(twai_message_t));

            // printf("SID: 0x%04lX, dlc: %d, data: %d,%d,%d,%d,%d,%d,%d,%d\n",rx_msg.identifier,rx_msg.data_length_code,rx_msg.data[0],rx_msg.data[1],rx_msg.data[2],rx_msg.data[3],rx_msg.data[4],rx_msg.data[5],rx_msg.data[6],rx_msg.data[7]);
            xQueueSendToBack(can_rx_to_master_queue, &ptr_msg, portMAX_DELAY);
        }
        else if ((rx_msg.identifier & CAN_CMD_TYPE_MASK) == CAN_BROADCAST) {
            can_process_cmd_broadcast(&rx_msg);
        }
    }
    return ESP_OK;
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

esp_err_t can_process_cmd_broadcast(twai_message_t *message)
{
    uint8_t cmd_num = CAN_SID_to_adress(message->identifier);
    printf("CAN BR; cmd_num: 0x%02X\n",cmd_num);
    return ESP_OK;
}


esp_err_t can_tx_cmd_broadcast(uint8_t cmd_number, uint8_t dlc, uint8_t data[TWAI_FRAME_MAX_DLC])
{
    // check params
    if(dlc > TWAI_FRAME_MAX_DLC || data == NULL)
        return ESP_FAIL;

    twai_message_t tx_message = {
        .identifier = CAN_BROADCAST | cmd_number, 
        .data_length_code = dlc,
        .ss = 1, 
        };
    memcpy(tx_message.data, data, dlc);

    esp_err_t status = twai_transmit(&tx_message, portMAX_DELAY);
    return status;
}

esp_err_t can_tx_cmd_to_slave(uint8_t slave_address, uint8_t cmd_number, uint8_t tx_data_len, uint8_t tx_data[TWAI_FRAME_MAX_DLC-1], can_cmd_status* rx_status, uint8_t* rx_data_len, uint8_t* rx_data)
{
    // check params
    if(tx_data_len > TWAI_FRAME_MAX_DLC-1 || tx_data == NULL)
        return ESP_FAIL;

    twai_message_t tx_message = {
        .identifier = CAN_TO_SLAVE | slave_address, 
        .data_length_code = tx_data_len + 1,
        .ss = 1, 
        };
    // first byte of payload is fixed and contains cmd number
    tx_message.data[0] = cmd_number;
    // rest is command specific
    if(tx_data_len > 0)
        memcpy(tx_message.data[0], tx_data, tx_data_len);

    // wait for transmit mutex -- only one tx+rx command at the time is supproted
    xSemaphoreTake(can_tx_mutex, portMAX_DELAY);   
    if(ESP_OK != twai_transmit(&tx_message, portMAX_DELAY))
    {
        // enable for tx other cmds
        xSemaphoreGive(can_tx_mutex);
        return ESP_FAIL; // tx fail
    }

    // wait for response
    TickType_t current_ticks = xTaskGetTickCount();
    twai_message_t* rx_msg;
    while(xTaskGetTickCount() < current_ticks + CAN_DEFAULT_RX_TIMEOUT)
    {
        // force receive task
        xSemaphoreGive(can_rx_sem);
        // check queue
        if(pdTRUE == xQueueReceive(can_rx_to_master_queue,&rx_msg,pdMS_TO_TICKS(100)))
        {
            // printf("Received from queue\n");
            // printf("SID: 0x%04lX, dlc: %d, data: %d,%d,%d,%d,%d,%d,%d,%d\n",rx_msg->identifier,rx_msg->data_length_code,rx_msg->data[0],rx_msg->data[1],rx_msg->data[2],rx_msg->data[3],rx_msg->data[4],rx_msg->data[5],rx_msg->data[6],rx_msg->data[7]);
            uint8_t rx_slave_address = CAN_SID_to_adress(rx_msg->identifier);
            // check correct slave
            if(rx_slave_address == slave_address)
            {
                // check payload length before reading memory
                if(rx_msg->data_length_code < 2)
                {
                    vPortFree(rx_msg);
                    xSemaphoreGive(can_tx_mutex);
                    return ESP_FAIL;
                }

                // check correct payload
                if(rx_msg->data[0] == cmd_number)
                {
                    // save status code 
                    *rx_status = rx_msg->data[1];
                    // save the response 
                    *rx_data_len = rx_msg->data_length_code - 2;
                    if(*rx_data_len > 0)
                        memcpy(rx_data, &(rx_msg->data[2]), *rx_data_len);
                    // always free the memory after reading from queue
                    vPortFree(rx_msg);
                    // enable for tx other cmds
                    xSemaphoreGive(can_tx_mutex);
                    return ESP_OK;
                }
            }
            // always free the memory after reading from queue
            vPortFree(rx_msg);
        }

    }
     // enable for tx other cmds
    xSemaphoreGive(can_tx_mutex);
    return ESP_FAIL; // no or wrong response
}


// TO SLAVE COMMANDS
esp_err_t can_slave_reset(uint8_t slave_address)
{
    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_RST,tx_data_len , tx_data,  &rx_status,&rx_data_len, &rx_data);
}

esp_err_t can_slave_restore_defaults(uint8_t slave_address)
{
    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_RES_DEF, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}

esp_err_t can_slave_mute(uint8_t slave_address, u_int8_t time_s)
{
    uint8_t tx_data_len = 1u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    
    tx_data[0] = time_s;
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_MUTE, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}

esp_err_t can_slave_ping(uint8_t slave_address)
{
    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_PING, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}

esp_err_t can_slave_get_SN(uint8_t slave_address, node_sn_t* sn)
{
    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_GET_SN, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}

esp_err_t can_slave_get_node_type(uint8_t slave_address, node_type_t* node_type)
{
    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_GET_NODE_TYPE, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}

esp_err_t can_slave_get_node_status(uint8_t slave_address, node_status_t* status)
{
uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_GET_STATUS, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}

esp_err_t can_slave_get_temperature(uint8_t slave_address, node_temp_t* temp)
{
uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_GET_TEMP, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}

esp_err_t can_slave_get_uptime(uint8_t slave_address, node_uptime_t* uptime)
{
    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;
    
    return can_tx_cmd_to_slave(slave_address, CAN_TS_GET_UPTIME, tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data);
}


esp_err_t can_driver_deinit()
{
    esp_err_t status = twai_stop();
    ESP_LOGI(TAG, "Driver stopped");
    status |= twai_driver_uninstall();
    ESP_LOGI(TAG, "Driver uninstalled");
    return status;
}
