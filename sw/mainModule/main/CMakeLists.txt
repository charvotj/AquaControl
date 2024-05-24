idf_component_register(SRCS "cJSON.c" "wifi_driver.c" "relays_driver.c" "water_level_sensor_driver.c" "temp_sensor_driver.c" "CAN_driver.c" "main.c" "debug_functions.c" "status_leds_driver.c" "status_control.c" "CAN_driver.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES isrgrootx1.pem)
