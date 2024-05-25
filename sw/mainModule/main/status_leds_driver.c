#include "status_leds_driver.h"

static led_strip_handle_t led_strip;
static float ledstrip_brightness;


esp_err_t status_leds_init(void)
{
    esp_err_t status = ESP_OK;

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_GPIO,
        .max_leds = LED_STRIP_LENGTH, 
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812, // LED strip model
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .flags.with_dma = true, // Using DMA can improve performance and help drive more LEDs
        .spi_bus = SPI2_HOST,   // SPI bus ID
    };

    // Custom Configuration
    status = status_leds_change_brightness(0.1);
    if(ESP_OK != status)
        return status;
    // TODO refactor status and erro check

    // status = led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip);
    status = led_strip_new_spi_device(&strip_config, &spi_config, &led_strip);
    if(ESP_OK != status)
        return status;

    /* Set all LED off to clear all pixels */
    status = led_strip_clear(led_strip);
    return status;

    
}

esp_err_t status_leds_set_color(status_leds_t led, led_color_t color)
{
    // adjust brightness 
    color.red *= ledstrip_brightness;
    color.green *= ledstrip_brightness;
    color.blue *= ledstrip_brightness;

    // printf("Set LED color\n");
   return led_strip_set_pixel(led_strip, led, color.red, color.green, color.blue);
}

esp_err_t status_leds_clear(void)
{
    return led_strip_clear(led_strip);
}

esp_err_t status_leds_change_brightness(float brightness)
{
    if(brightness < 0 || brightness > 1)
        return ESP_ERR_INVALID_ARG;
    
    ledstrip_brightness = brightness;
    return ESP_OK;
}


esp_err_t status_leds_update()
{
    switch (STATUS_device)
    {
    case DEVST_NORMAL:
        status_leds_set_color(POWER_LED,COLOR_GREEN);
        break;
    case DEVST_ERROR_GENERIC:
    case DEVST_ERROR_WIFI:
    case DEVST_ERROR_CAN_BUS:
        status_leds_set_color(POWER_LED,COLOR_RED);
        break;
    case DEVST_STARTUP:
        status_leds_set_color(POWER_LED, COLOR_MAGENTA);
        break;
    case DEVST_UNDEFINED:
        status_leds_set_color(POWER_LED, COLOR_BLUE);
        break;
    default:
        return ESP_FAIL;
    }

    switch (STATUS_wifi)
    {
    case WIFIST_ONLINE:
        status_leds_set_color(WIFI_LED,COLOR_GREEN);
        break;
    case WIFIST_CONNECTED:
        status_leds_set_color(WIFI_LED,COLOR_ORANGE);
        break;
    case WIFIST_ERROR:
        status_leds_set_color(WIFI_LED,COLOR_RED);
        break;
    case WIFIST_STARTUP:
        status_leds_set_color(WIFI_LED, COLOR_MAGENTA);
        break;
    case WIFIST_UNDEFINED:
        status_leds_set_color(WIFI_LED, COLOR_BLUE);
        break;
    default:
        return ESP_FAIL;
    }

    // default
    status_leds_set_color(ALARM_LED,COLOR_BLACK);
    status_leds_set_color(USER_LED,COLOR_BLACK);

    /* Refresh the strip to send data */
    return led_strip_refresh(led_strip);
}