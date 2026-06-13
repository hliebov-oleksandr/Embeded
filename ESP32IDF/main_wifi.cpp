#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2

static const char *TAG = "WEB_SERVER";

// обработчик событий Wi-Fi
static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{

    ESP_LOGI("HANDLER", "event_base=%s event_id=%d", event_base, event_id);
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

        char ip_str[16];
        char netmask_str[16];
        char gw_str[16];

        esp_ip4addr_ntoa(&event->ip_info.ip, ip_str, sizeof(ip_str));
        esp_ip4addr_ntoa(&event->ip_info.netmask, netmask_str, sizeof(netmask_str));
        esp_ip4addr_ntoa(&event->ip_info.gw, gw_str, sizeof(gw_str));

        ESP_LOGI("WIFI", "Got IP: %s", ip_str);
        ESP_LOGI("WIFI", "Netmask: %s", netmask_str);
        ESP_LOGI("WIFI", "Gateway: %s", gw_str);
    }
}

static const char *html_page =
    "<html><body><h1>ESP32-S3 Control</h1>"
    "<button onclick=\"location.href='/on'\">ON</button>"
    "<button onclick=\"location.href='/off'\">OFF</button>"
    "</body></html>";

esp_err_t on_handler(httpd_req_t *req)
{
    gpio_set_level(LED_PIN, 1);
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t off_handler(httpd_req_t *req)
{
    gpio_set_level(LED_PIN, 0);
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t root_handler(httpd_req_t *req)
{
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.uri_match_fn = httpd_uri_match_wildcard;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t uri_root = {.uri = "/", .method = HTTP_GET, .handler = root_handler};
        httpd_register_uri_handler(server, &uri_root);

        httpd_uri_t uri_on = {.uri = "/on", .method = HTTP_GET, .handler = on_handler};
        httpd_register_uri_handler(server, &uri_on);

        httpd_uri_t uri_off = {.uri = "/off", .method = HTTP_GET, .handler = off_handler};
        httpd_register_uri_handler(server, &uri_off);

        ESP_LOGI(TAG, "Webserver started");
    }
    else
    {
        ESP_LOGI(TAG, "Webserver is not started");
    }
}

void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "myESP",
            .password = "aaaBBB123",
            .ssid_len = strlen("myEsp"),
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .max_connection = 4},
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    // ESP_ERROR_CHECK(esp_wifi_connect());
}

void wifi_init_sta(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "*julia-porosenok",
            .password = "aaaBBB123"},
    };
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        NULL));

    // ПРОБЛЕМА БЫЛА ЗДЕСЬ:
    // раньше вызывался esp_wifi_connect() до запуска драйвера
    // нужно сначала esp_wifi_start(), потом esp_wifi_connect()
    ESP_LOGI("WIFI", "WIFI START %d", esp_wifi_start());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI("WIFI", "WIFI CONNECT %d", esp_wifi_connect());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

extern "C" void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    wifi_init_sta();
    start_webserver();

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}