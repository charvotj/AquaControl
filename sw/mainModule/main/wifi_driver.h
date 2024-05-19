#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "string.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_http_client.h"


#include "pins.h"
#include "status_control.h"


#define WIFI_SSID                 "Milenium Falcon"
#define WIFI_PASS                 "hansolojetop"
#define MAX_HTTP_RECV_BUFFER      512
#define WIFI_CONN_MAX_RETRY       5

esp_err_t wifi_driver_routine();
void wifi_init_sta(void);
esp_err_t _http_event_handler(esp_http_client_event_t *evt);


#endif /* WIFI_DRIVER_H*/
