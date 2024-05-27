#ifndef TIME_DRIVER_H
#define TIME_DRIVER_H

#include "driver/gpio.h"
#include "esp_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"

typedef struct
{
    uint8_t	s;
    uint8_t	m;
    uint8_t	h;
    uint8_t	day;
    uint8_t	month;
    uint8_t	year;
} simple_time_t;


esp_err_t is_time_set(void);
esp_err_t sync_ntp_time(void);
void set_timezone(const char *timezone);
void print_time();
esp_err_t time_now_simple(simple_time_t* now_simple);





#endif /* TIME_DRIVER_H*/
