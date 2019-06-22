#include <stdio.h>
#include "esp_system.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_err.h"
#include "internal/esp_wifi_internal.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/pwm.h"
#include "cJSON.h"
#include "rom/ets_sys.h"
#include "driver/uart.h"
#include "ws2812_driver.hpp"
#include "tcpip_adapter.h"
#include "esp_smartconfig.h"

//打印日志用
static const char *TAG = "Ws2812Log";

/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    printf("\n\n-------------------------------- Get Systrm Info------------------------------------------\n");
    //获取IDF版本
    printf("     SDK version:%s\n", esp_get_idf_version());
    //获取芯片可用内存
    printf("     esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
    //获取从未使用过的最小内存
    printf("     esp_get_minimum_free_heap_size : %d  \n", esp_get_minimum_free_heap_size());
    //获取芯片的内存分布，返回值具体见结构体 flash_size_map
    printf("     system_get_flash_size_map(): %d \n", system_get_flash_size_map());
    //获取mac地址（station模式）
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    printf("esp_read_mac(): %02x:%02x:%02x:%02x:%02x:%02x \n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf("--------------------------------------------------------------------------\n\n");
    WS2812_Init();
    while (1)
    {
        setAllPixelColor(254, 0, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        setAllPixelColor(0, 254, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        setAllPixelColor(0, 0, 254);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

//python /home/XuHongYss/ESP8266_RTOS_SDK/components/esptool_py/esptool/esptool.py --chip esp8266 --port COM12 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 2MB 0x0000 /home/XuHongYss/ESP8266_RTOS_SDK/MyProject/ESP8266_WS2812/build/bootloader/bootloader.bin 0x10000 /home/XuHongYss/ESP8266_RTOS_SDK/MyProject/ESP8266_WS2812/build/simple_wifi.bin 0x8000 /home/XuHongYss/ESP8266_RTOS_SDK/MyProject/ESP8266_WS2812/build/partitions_singleapp.bin