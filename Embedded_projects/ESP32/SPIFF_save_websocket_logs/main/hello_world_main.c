#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include <string.h>
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_http_server.h"

#define EXAMPLE_ESP_WIFI_SSID "Ganesh"
#define EXAMPLE_ESP_WIFI_PASS "Ganesh@123"
#define EXAMPLE_ESP_WIFI_CHANNEL 1
#define EXAMPLE_MAX_STA_CONN 4

static const char *TAG = "wifi softAP";

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d, reason=%d",
                 MAC2STR(event->mac), event->aid, event->reason);
    }
}

void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
            .authmode = WIFI_AUTH_WPA3_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
#else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
            .authmode = WIFI_AUTH_WPA2_PSK,
#endif
            .pmf_cfg = {
                .required = true,
            },
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void init_spiffs(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs", // mount point
        .partition_label = NULL,
        .max_files = 5, // max number of open files
        .format_if_mount_failed = true};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE("SPIFFS", "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE("SPIFFS", "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE("SPIFFS", "Failed to init SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret == ESP_OK)
    {
        ESP_LOGI("SPIFFS", "Partition size: total: %d, used: %d", total, used);
    }
    else
    {
        ESP_LOGE("SPIFFS", "Failed to get SPIFFS partition info (%s)", esp_err_to_name(ret));
    }
}

// void test_spiffs(void)
// {
//     // Writing to file
//     FILE *f = fopen("/spiffs/hello.txt", "w");
//     if (f == NULL)
//     {
//         ESP_LOGE("SPIFFS", "Failed to open file for writing");
//         return;
//     }
//     fprintf(f, "Hello Gani!\n");
//     fclose(f);
//     ESP_LOGI("SPIFFS", "File written");

//     // Reading from file
//     f = fopen("/spiffs/hello.txt", "r");
//     if (f == NULL)
//     {
//         ESP_LOGE("SPIFFS", "Failed to open file for reading");
//         return;
//     }
//     char line[64];
//     fgets(line, sizeof(line), f);
//     fclose(f);
//     ESP_LOGI("SPIFFS", "Read: '%s'", line);
// }
void append_to_spiffs(const char *web_data)
{
    // Step 1: Append new data
    FILE *f = fopen("/spiffs/hello.txt", "a");
    if (f == NULL)
    {
        ESP_LOGE("SPIFFS", "Failed to open file for appending");
        return;
    }

    fprintf(f, "%s\n", web_data);
    ESP_LOGI("SPIFFS", "Appended: '%s'", web_data);
    fclose(f);

    // f = fopen("/spiffs/hello.txt", "r");
    // if (f == NULL)
    // {
    //     ESP_LOGE("SPIFFS", "Failed to open file for reading");
    //     return;
    // }

    // ESP_LOGI("hello.txt", "File contents:");
    // char line[128]; // buffer for each line
    // while (fgets(line, sizeof(line), f) != NULL)
    // {
    //     // Remove newline for cleaner log
    //     line[strcspn(line, "\n")] = 0;
    //     ESP_LOGI("SPIFFS", "%s", line);
    // }

    fclose(f);
}

const char *html_page =
"<!DOCTYPE html><html><head><title>ESP32</title>"
"<style>"
"body {"
"  font-family: 'Segoe UI', sans-serif;"
"  background: linear-gradient(to right, #1e3c72, #2a5298);"
"  color: #fff;"
"  text-align: center;"
"  padding-top: 50px;"
"}"
"h2 {"
"  font-size: 2em;"
"  margin-bottom: 20px;"
"}"
"form, button {"
"  margin: 10px auto;"
"}"
"input[type='text'] {"
"  padding: 10px;"
"  border-radius: 5px;"
"  border: none;"
"  width: 250px;"
"}"
"button {"
"  background: linear-gradient(to bottom, #4CAF50, #2E7D32);"
"  color: white;"
"  border: none;"
"  padding: 10px 20px;"
"  text-shadow: 1px 1px 2px #000;"
"  box-shadow: 0 4px 6px rgba(0,0,0,0.3);"
"  border-radius: 8px;"
"  font-weight: bold;"
"  cursor: pointer;"
"  transition: transform 0.2s;"
"}"
"button:hover {"
"  transform: scale(1.05);"
"}"
"#response {"
"  margin-top: 20px;"
"  font-size: 1.2em;"
"  white-space: pre-wrap;"
"}"
"</style></head><body>"
"<h2>Intelli Aviotz </h2>"
"<p> you are connected to ESP32 Web Server</p>"
"<form id='dataForm'>"
"<input type='text' id='message' name='message' required>"
"<button type='submit'>Send</button>"
"</form><br>"
"<button onclick='readFile()'>Read File</button>"
"<button onclick='eraseFile()'>Erase File</button>"
"<button onclick=\"window.location.href='/download'\">Download logs.txt</button>"
"<p id='response'></p>"
"<script>"
"document.getElementById('dataForm').addEventListener('submit', function(e) {"
"e.preventDefault();"
"const msg = document.getElementById('message').value;"
"fetch('/submit', {"
"method: 'POST',"
"headers: { 'Content-Type': 'text/plain' },"
"body: msg"
"}).then(response => response.text()).then(data => {"
"document.getElementById('response').innerText = 'ESP32 says: ' + data;"
"document.getElementById('message').value = '';"
"}).catch(error => {"
"document.getElementById('response').innerText = 'Error: ' + error;"
"});"
"});"
"function readFile() {"
"fetch('/read').then(response => response.text()).then(data => {"
"document.getElementById('response').innerText = 'File Contents:\\n' + data;"
"}).catch(error => {"
"document.getElementById('response').innerText = 'Error: ' + error;"
"});"
"}"
"function eraseFile() {"
"fetch('/erase', { method: 'POST' }).then(response => response.text()).then(data => {"
"document.getElementById('response').innerText = data;"
"}).catch(error => {"
"document.getElementById('response').innerText = 'Error: ' + error;"
"});"
"}"
"</script></body></html>";

esp_err_t download_handler(httpd_req_t *req)
{
    // Tell browser to download instead of display
    httpd_resp_set_type(req, "text/plain");
    httpd_resp_set_hdr(req, "Content-Disposition", "attachment; filename=\"logs.txt\"");

    // Open file from SPIFFS
    FILE *file = fopen("/spiffs/hello.txt", "r");
    if (file == NULL)
    {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
        return ESP_FAIL;
    }

    char buffer[128];
    size_t read_bytes;
    while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) // fread read the given amount if data from a file
    {
        if (httpd_resp_send_chunk(req, buffer, read_bytes) != ESP_OK)
        {
            fclose(file);
            httpd_resp_sendstr_chunk(req, NULL); // end response
            return ESP_FAIL;
        }
    }
    fclose(file);

    // End response
    httpd_resp_sendstr_chunk(req, NULL);

    return ESP_OK;
}

esp_err_t read_handler(httpd_req_t *req)
{
    FILE *f = fopen("/spiffs/hello.txt", "r");
    if (f == NULL)
    {
        httpd_resp_send(req, "File not found", HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }

    char line[256];
    size_t total = 0;
    httpd_resp_set_type(req, "text/plain");

    while (fgets(line, sizeof(line), f)) // fgets return only 1st line of the file
    {
        httpd_resp_send_chunk(req, line, strlen(line));
        total += strlen(line);
    }

    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0);
    ESP_LOGI("READ", "Sent %d bytes", total);
    return ESP_OK;
}

esp_err_t erase_handler(httpd_req_t *req)
{
    esp_err_t res = remove("/spiffs/hello.txt") == 0 ? ESP_OK : ESP_FAIL;
    if (res == ESP_OK)
    {
        httpd_resp_send(req, "File erased!", HTTPD_RESP_USE_STRLEN);
    }
    else
    {
        httpd_resp_send(req, "Failed to erase file", HTTPD_RESP_USE_STRLEN);
    }
    return res;
}

esp_err_t get_handler(httpd_req_t *req)
{
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0)
    {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    buf[ret] = '\0'; // Null-terminate

    ESP_LOGI("POST", "Received plain text: %s", buf);
    append_to_spiffs(buf);
    httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t uri_get = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = get_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &uri_get);

        httpd_uri_t uri_post = {
            .uri = "/submit",
            .method = HTTP_POST,
            .handler = post_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &uri_post);
        httpd_uri_t uri_read = {
            .uri = "/read",
            .method = HTTP_GET,
            .handler = read_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &uri_read);

        httpd_uri_t uri_erase = {
            .uri = "/erase",
            .method = HTTP_POST,
            .handler = erase_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &uri_erase);
        httpd_uri_t uri_download = {
            .uri = "/download",
            .method = HTTP_GET,
            .handler = download_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &uri_download);
    }
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    printf("Hello world!\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    init_spiffs();
    // test_spiffs();

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
    start_webserver();
}
