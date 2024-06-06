#define DEVICE_TYPE DEVICE_TYPE_MASTER // << DO NOT CHANGE THIS

#define FW_VERSION_MAJOR    0
#define FW_VERSION_MINOR    2

#define SERIAL_NUMBER       (uint32_t)44u

#define CONFIG_MAX_CONNECTED_NODES          255u

#define CONFIG_DEFAULT_WEB_DATA_SEND_PERIOD pdMS_TO_TICKS(20000)

#define CONFIG_DEFAULT_WIFI_CONN_CHECK_PER    pdMS_TO_TICKS(5000)
#define CONFIG_MAX_WIFI_CONN_CHECK_PER        pdMS_TO_TICKS(30000)
#define CONFIG_DEFAULT_WIFI_CONFIG_CHECK_PER  pdMS_TO_TICKS(20000)
#define CONFIG_DEFAULT_WIFI_NTP_CHECK_PER     pdMS_TO_TICKS(60*60*1000)


#define RELAYS_NUM  4u   // number of relays available