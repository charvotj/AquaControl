/* 
   TODO: License or other funny thing
*/




#include "driver/gpio.h"
#include "esp_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#include "pins.h"
#include "debug_functions.h"
#include "status_leds_driver.h"
#include "status_control.h"
#include "CAN_driver.h"


#define STATS_TASK_PRIO     3
#define STATS_TICKS         pdMS_TO_TICKS(1000)


static SemaphoreHandle_t sync_control_task;
static SemaphoreHandle_t sync_status_update_task;
static SemaphoreHandle_t sync_status_task_init_done;
static SemaphoreHandle_t sync_fw_OTA_task;
static SemaphoreHandle_t sync_wifi_task;
static SemaphoreHandle_t sync_can_bus_task;
static SemaphoreHandle_t sync_stats_task;
static SemaphoreHandle_t sync_display_task;

static const char *TAG = "mainBoard - main.c";


#define CONFIG_BLINK_PERIOD 500

static uint8_t s_led_state = 0;


static void control_task(void *arg)
{
    xSemaphoreTake(sync_control_task, portMAX_DELAY);
    ESP_LOGI(TAG,"Entering task: control_task");

    ESP_LOGI(TAG,"control_task -- starting others");
    xSemaphoreGive(sync_status_update_task);
    // Wait for status control init
    ESP_LOGI(TAG,"control_task -- waiting for init done");
    xSemaphoreTake(sync_status_task_init_done, portMAX_DELAY);
    // Start other tasks 
    xSemaphoreGive(sync_wifi_task);
    xSemaphoreGive(sync_fw_OTA_task);
    xSemaphoreGive(sync_can_bus_task);
    xSemaphoreGive(sync_display_task);

    while (1) {
        // TODO
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



static void status_update_task(void *arg)
{
    xSemaphoreTake(sync_status_update_task, portMAX_DELAY);
    ESP_LOGI(TAG,"Entering task: status_update_task");

    ESP_ERROR_CHECK(status_control_init());
    // Notify about init done - unblock other tasks
    xSemaphoreGive(sync_status_task_init_done);
    ESP_ERROR_CHECK(status_leds_init());
    status_leds_update(STATUS_device);

    ESP_LOGI(TAG,"Status update task - %d \n", STATUS_device);

    while (1) {
        // TODO
        device_status_t old_status = STATUS_device;
        process_device_status();
        if(old_status != STATUS_device)
        {
            printf("Device status changed to %d\n", STATUS_device);
            status_leds_update(STATUS_device);
        }
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

static void fw_OTA_task(void *arg)
{
    xSemaphoreTake(sync_fw_OTA_task, portMAX_DELAY);
    ESP_LOGI(TAG,"Entering task: fw_OTA_task");
    // initialization
    set_ota_module_status(MODST_READY);

    while (1) {
        // TODO
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void wifi_task(void *arg)
{
    xSemaphoreTake(sync_wifi_task, portMAX_DELAY);
    ESP_LOGI(TAG,"Entering task: wifi_task");

    while (1) {
        // TODO
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void can_bus_task(void *arg)
{
    xSemaphoreTake(sync_can_bus_task, portMAX_DELAY);
    ESP_LOGI(TAG,"Entering task: can_bus_task");
    // initialization
    set_can_module_status(MODST_STARTUP);
    ESP_ERROR_CHECK(can_driver_init());
    set_can_module_status(MODST_READY);

    while (1) 
    {
        can_proccess_rx(100 / portTICK_PERIOD_MS);
    }

    ESP_ERROR_CHECK(can_driver_deinit());
    vTaskDelete(NULL);
}

static void display_task(void *arg)
{
    xSemaphoreTake(sync_display_task, portMAX_DELAY);
    ESP_LOGI(TAG,"Entering task: display_task");
    // initialization
    set_display_module_status(MODST_READY);

    while (1) {
        // TODO
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


static void stats_task(void *arg)
{
    xSemaphoreTake(sync_stats_task, portMAX_DELAY);
    ESP_LOGI(TAG,"Entering task: stats_task");

    //Print real time stats periodically
    while (1) {
        printf("\n\nGetting real time stats over %"PRIu32" ticks\n", STATS_TICKS);
        esp_err_t statusCode = print_real_time_stats(STATS_TICKS); 
        if (statusCode == ESP_OK) {
            printf("Real time stats obtained\n");
        } 
        else if (statusCode == ESP_ERR_INVALID_SIZE){
            printf("Invalid size :(\n");
        }
        
        else {
            printf("Error getting real time stats: %d\n",statusCode);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void app_main(void)
{
    //Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));

    //Create semaphores to synchronize
    sync_stats_task = xSemaphoreCreateBinary();
    sync_status_update_task = xSemaphoreCreateBinary();
    sync_wifi_task = xSemaphoreCreateBinary();
    sync_fw_OTA_task = xSemaphoreCreateBinary();
    sync_can_bus_task = xSemaphoreCreateBinary();
    sync_display_task = xSemaphoreCreateBinary();
    sync_control_task = xSemaphoreCreateBinary();
    sync_status_task_init_done = xSemaphoreCreateBinary();


    // TODO: update priorities
    //Create stats task
    xTaskCreatePinnedToCore(stats_task, "stats", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
    //Create status task
    xTaskCreatePinnedToCore(status_update_task, "status update", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
    //Create wifi task
    xTaskCreatePinnedToCore(wifi_task, "wifi", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
    //Create OTA task
    xTaskCreatePinnedToCore(fw_OTA_task, "fw_OTA_task", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
    //Create CAN task
    xTaskCreatePinnedToCore(can_bus_task, "can_bus_task", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
    //Create status task
    xTaskCreatePinnedToCore(display_task, "display_task", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
    //Create control task
    xTaskCreatePinnedToCore(control_task, "control_task", 4096, NULL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);

    
    //Start control task
    xSemaphoreGive(sync_control_task);
    // xSemaphoreGive(sync_stats_task);
}
