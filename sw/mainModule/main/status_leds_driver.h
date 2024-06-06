#ifndef STATUS_LEDS_DRIVER_H
#define STATUS_LEDS_DRIVER_H


#include "driver/gpio.h"
#include "esp_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#include "led_strip.h"

#include "pins.h"
#include "status_control.h"

#define LED_STRIP_LENGTH 4

typedef struct {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
} led_color_t;

typedef enum {
    POWER_LED = 0,
    WIFI_LED,
    ALARM1_LED,
    ALARM2_LED
} status_leds_t;

static const led_color_t COLOR_RED = {255, 0, 0};
static const led_color_t COLOR_ORANGE = {255, 165, 0};
static const led_color_t COLOR_YELLOW = {255, 255, 0};
static const led_color_t COLOR_GREEN = {0, 255, 0};
static const led_color_t COLOR_BLUE = {0, 0, 255};
static const led_color_t COLOR_CYAN = {0, 255, 255};
static const led_color_t COLOR_MAGENTA = {255, 0, 255};
static const led_color_t COLOR_WHITE = {255, 255, 255};
static const led_color_t COLOR_PURPLE = {128, 0, 128};
static const led_color_t COLOR_BLACK = {0, 0, 0};

esp_err_t status_leds_init(void);
esp_err_t status_leds_set_color(status_leds_t led, led_color_t color);
esp_err_t status_leds_clear(void);
esp_err_t status_leds_change_brightness(float brightness);
esp_err_t status_leds_update();




#endif // STATUS_LEDS_DRIVER_H