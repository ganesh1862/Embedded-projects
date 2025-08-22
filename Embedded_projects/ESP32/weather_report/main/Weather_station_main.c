#include "cJSON.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include <string.h>

#define EXAMPLE_ESP_WIFI_SSID "NANDY_STA_UP"
#define EXAMPLE_ESP_WIFI_PASS "@Intelli_Aviotz$"
#define EXAMPLE_ESP_MAXIMUM_RETRY 10

#if CONFIG_ESP_WPA3_SAE_PWE_HUNT_AND_PECK
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HUNT_AND_PECK
#define EXAMPLE_H2E_IDENTIFIER ""
#elif CONFIG_ESP_WPA3_SAE_PWE_HASH_TO_ELEMENT
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HASH_TO_ELEMENT
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#elif CONFIG_ESP_WPA3_SAE_PWE_BOTH
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#endif

#if CONFIG_ESP_WIFI_AUTH_OPEN
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ESP_WIFI_AUTH_WEP
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ESP_WIFI_AUTH_WPA_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WAPI_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char *TAG = "wifi weather station";
#define MAX_HTTP_OUTPUT_BUFFER 2048
#define API_KEY "3735dbacceb7e04e1bccc389dbb72f2a" 

static char http_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
static int response_len = 0;

// Function declarations
static void parse_weather_response(const char *json_string);
static esp_err_t _http_event_handler(esp_http_client_event_t *evt);
void fetch_weather_data(void);
void wifi_init_sta(void);


static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (response_len + evt->data_len < MAX_HTTP_OUTPUT_BUFFER) {
                memcpy(http_response_buffer + response_len, evt->data, evt->data_len);
                response_len += evt->data_len;
            } else {
                ESP_LOGE(TAG, "Response buffer overflow");
                response_len = 0;
                memset(http_response_buffer, 0, sizeof(http_response_buffer));
            }
            break;
            
        case HTTP_EVENT_ON_FINISH:
            if (response_len > 0) {
                http_response_buffer[response_len] = '\0';
                parse_weather_response(http_response_buffer);
            }
            response_len = 0;
            memset(http_response_buffer, 0, sizeof(http_response_buffer));
            break;
            
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "Connected to server");
            response_len = 0;
            memset(http_response_buffer, 0, sizeof(http_response_buffer));
            break;
            
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from server");
            break;
            
        default:
            break;
    }
    return ESP_OK;
}

static void parse_weather_response(const char *json_string) {
	cJSON *root = cJSON_Parse(json_string);
	if (root == NULL) {
		ESP_LOGE(TAG, "Failed to parse JSON response");
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			ESP_LOGE(TAG, "JSON Error: %s", error_ptr);
		}
		return;
	}

	ESP_LOGI(TAG, "========== WEATHER REPORT ==========");

	// Parse city name and country
	cJSON *name_item = cJSON_GetObjectItem(root, "name");
	cJSON *sys = cJSON_GetObjectItem(root, "sys");
	if (cJSON_IsString(name_item)) {
		if (sys != NULL) {
			cJSON *country_item = cJSON_GetObjectItem(sys, "country");
			if (cJSON_IsString(country_item)) {
				ESP_LOGI(TAG, "City: %s, %s", name_item->valuestring,
						 country_item->valuestring);
			} else {
				ESP_LOGI(TAG, "City: %s", name_item->valuestring);
			}
		} else {
			ESP_LOGI(TAG, "City: %s", name_item->valuestring);
		}
	}

	// Parse timestamp and timezone for local time
	cJSON *dt_item = cJSON_GetObjectItem(root, "dt");
	cJSON *timezone_item = cJSON_GetObjectItem(root, "timezone");

	if (cJSON_IsNumber(dt_item) && cJSON_IsNumber(timezone_item)) {
		time_t timestamp = (time_t)dt_item->valueint;
		int timezone_offset =
			timezone_item->valueint; // offset in seconds from UTC

		// Convert to local time
		time_t local_time = timestamp + timezone_offset;
		struct tm *time_info = gmtime(&local_time);

		char time_str[100];
		strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
		ESP_LOGI(TAG, "Local Time: %s (UTC%+d)", time_str,
				 timezone_offset / 3600);
	}

	// Parse main weather data
	cJSON *main = cJSON_GetObjectItem(root, "main");
	if (main != NULL) {
		cJSON *temp_item = cJSON_GetObjectItem(main, "temp");
		cJSON *humidity_item = cJSON_GetObjectItem(main, "humidity");
		cJSON *pressure_item = cJSON_GetObjectItem(main, "pressure");
		cJSON *feels_like_item = cJSON_GetObjectItem(main, "feels_like");

		if (cJSON_IsNumber(temp_item)) {
			double temp = temp_item->valuedouble;
			ESP_LOGI(TAG, "Temperature: %.1f °C", temp);
		}

		if (cJSON_IsNumber(feels_like_item)) {
			double feels_like = feels_like_item->valuedouble;
			ESP_LOGI(TAG, "Feels like: %.1f °C", feels_like);
		}

		if (cJSON_IsNumber(humidity_item)) {
			int humidity = humidity_item->valueint;
			ESP_LOGI(TAG, "Humidity: %d%%", humidity);
		}

		if (cJSON_IsNumber(pressure_item)) {
			int pressure = pressure_item->valueint;
			ESP_LOGI(TAG, "Pressure: %d hPa", pressure);
		}
	}

	// Parse weather description
	cJSON *weather_array = cJSON_GetObjectItem(root, "weather");
	if (cJSON_IsArray(weather_array)) {
		cJSON *weather_item = cJSON_GetArrayItem(weather_array, 0);
		if (weather_item != NULL) {
			cJSON *description =
				cJSON_GetObjectItem(weather_item, "description");
			cJSON *main_weather = cJSON_GetObjectItem(weather_item, "main");

			if (cJSON_IsString(main_weather) && cJSON_IsString(description)) {
				ESP_LOGI(TAG, "Condition: %s (%s)", main_weather->valuestring,
						 description->valuestring);
			} else if (cJSON_IsString(description)) {
				ESP_LOGI(TAG, "Condition: %s", description->valuestring);
			}
		}
	}

	// Parse wind data
	cJSON *wind = cJSON_GetObjectItem(root, "wind");
	if (wind != NULL) {
		cJSON *speed = cJSON_GetObjectItem(wind, "speed");
		cJSON *deg = cJSON_GetObjectItem(wind, "deg");

		if (cJSON_IsNumber(speed)) {
			double wind_speed = speed->valuedouble;
			if (cJSON_IsNumber(deg)) {
				int wind_dir = deg->valueint;
				const char *direction = "";
				if (wind_dir >= 337.5 || wind_dir < 22.5)
					direction = "N";
				else if (wind_dir < 67.5)
					direction = "NE";
				else if (wind_dir < 112.5)
					direction = "E";
				else if (wind_dir < 157.5)
					direction = "SE";
				else if (wind_dir < 202.5)
					direction = "S";
				else if (wind_dir < 247.5)
					direction = "SW";
				else if (wind_dir < 292.5)
					direction = "W";
				else if (wind_dir < 337.5)
					direction = "NW";

				ESP_LOGI(TAG, "Wind: %.1f m/s %s (%d°)", wind_speed, direction,
						 wind_dir);
			} else {
				ESP_LOGI(TAG, "Wind Speed: %.1f m/s", wind_speed);
			}
		}
	}

	// Parse visibility
	cJSON *visibility = cJSON_GetObjectItem(root, "visibility");
	if (cJSON_IsNumber(visibility)) {
		int vis = visibility->valueint;
		ESP_LOGI(TAG, "Visibility: %.1f km", vis / 1000.0);
	}

	ESP_LOGI(TAG, "====================================");

	cJSON_Delete(root);
}

void fetch_weather_data(void) {
    ESP_LOGI(TAG, "Starting weather data fetch...");

    char url[512];
    snprintf(url, sizeof(url),
             "http://api.openweathermap.org/data/2.5/"  
             "weather?q=Bengaluru&appid=%s&units=metric",
             API_KEY);

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
        .timeout_ms = 10000,
        .buffer_size = MAX_HTTP_OUTPUT_BUFFER,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return;
    }

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status_code = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "HTTPS Status = %d", status_code);
    } else {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    memset(http_response_buffer, 0, sizeof(http_response_buffer)); // Clear buffer
    response_len = 0;
}

static int s_retry_num = 0;

static void event_handler(void *arg, esp_event_base_t event_base,
						  int32_t event_id, void *event_data) {
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT &&
			   event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(TAG, "retry to connect to the AP");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGI(TAG, "connect to the AP fail");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
		ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

void wifi_init_sta(void) {
	s_wifi_event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());

	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(
		WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(
		IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

	wifi_config_t wifi_config = {
		.sta =
			{
				.ssid = EXAMPLE_ESP_WIFI_SSID,
				.password = EXAMPLE_ESP_WIFI_PASS,
				/* Authmode threshold resets to WPA2 as default if password
				 * matches WPA2 standards (password len => 8). If you want to
				 * connect the device to deprecated WEP/WPA networks, Please set
				 * the threshold value to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and
				 * set the password with length and format matching to
				 * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
				 */
				.threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
				.sae_pwe_h2e = ESP_WIFI_SAE_MODE,
				.sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
			},
	};
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(TAG, "wifi_init_sta finished.");

	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT)
	 * or connection failed for the maximum number of re-tries (WIFI_FAIL_BIT).
	 * The bits are set by event_handler() (see above) */
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
										   WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
										   pdFALSE, pdFALSE, portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we
	 * can test which event actually happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
				 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
				 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
	} else {
		ESP_LOGE(TAG, "UNEXPECTED EVENT");
	}
}

void weather_task(void *pvParameters) {
	while (1) {
		// Wait for 30 seconds before first fetch and between subsequent fetches
		vTaskDelay(pdMS_TO_TICKS(30000));

		// Fetch weather data
		fetch_weather_data();

		// Wait 1 minute before next fetch (60 seconds)
		vTaskDelay(pdMS_TO_TICKS(60000));
	}
}

void app_main(void) {
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
		ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
	wifi_init_sta();

	// Create weather fetch task
	xTaskCreate(weather_task, "weather_task", 8192, NULL, 5, NULL);
}