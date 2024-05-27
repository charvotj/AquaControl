#include "time_driver.h"

static const char *TAG = "mainBoard - time_driver.c";


esp_err_t is_time_set(void) 
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    return ((timeinfo.tm_year + 1900) > 2023)? ESP_OK : ESP_FAIL;
}

void time_sync_notification_cb(struct timeval *tv) {
    ESP_LOGI("SNTP", "Time synchronization event");
}



esp_err_t sync_ntp_time(void) 
{
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");

    config.sync_cb = time_sync_notification_cb;
    config.smooth_sync = false;
    
    esp_netif_sntp_init(&config);

    int retry = 0;
    const int retry_count = 3;
    while (esp_netif_sntp_sync_wait(500 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    esp_netif_sntp_deinit();

    if(retry == retry_count)
        return ESP_FAIL;

    return is_time_set();
}

void set_timezone(const char *timezone) {
    setenv("TZ", timezone, 1);
    tzset();
}

void print_time()
{
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time is: %s", strftime_buf);
}


esp_err_t time_now_simple(simple_time_t* now_simple)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    if(timeinfo.tm_year <= 2023)
        return ESP_FAIL;
    
    now_simple->s = timeinfo.tm_sec;
    now_simple->m = timeinfo.tm_min;
    now_simple->h = timeinfo.tm_hour;
    now_simple->day = timeinfo.tm_mday;
    now_simple->month = timeinfo.tm_mon;
    now_simple->year = timeinfo.tm_year;

    return ESP_OK;
}
