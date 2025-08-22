ESP32 Weather Station
A firmware project for the ESP32 microcontroller that connects to Wi-Fi, fetches real-time weather data from the OpenWeatherMap API, and displays a formatted report over the serial console. Built using the ESP-IDF framework.

Features
Wi-Fi Connectivity: Connects to a designated Wi-Fi access point using WPA2.

RESTful API Integration: Makes HTTP GET requests to the OpenWeatherMap API.

JSON Parsing: Efficiently parses complex JSON responses using the cJSON library.

Comprehensive Weather Data: Displays a wide range of meteorological information:

City name and country

Local timestamp with timezone adjustment

Current temperature and "feels like" temperature (°C)

Humidity (%) and atmospheric pressure (hPa)

Weather conditions (e.g., "Clouds" with a description like "overcast clouds")

Wind speed (m/s) and compass direction

Visibility (km)

Automated & Periodic: Fetches new data automatically every 60 seconds after an initial 30-second delay.

Robust Error Handling: Includes retry logic for Wi-Fi connection and detailed error logging for HTTP and JSON parsing.

=Hardware Requirements
An ESP32 development board (e.g., ESP32-WROOM-32, ESP32-DevKitC).
A USB cable for power, flashing, and serial communication.

Upon successful execution.
I (1950) esp_netif_handlers: sta ip: 192.168.1.42, mask: 255.255.255.0, gw: 192.168.1.1
I (1950) wifi weather station: got ip:192.168.1.42
I (1950) wifi weather station: connected to ap SSID:GANESH password:123456789
I (1960) main_task: Returned from app_main()
I (31960) wifi weather station: Starting weather data fetch...
I (32010) wifi weather station: Connected to server
I (32060) wifi weather station: ========== WEATHER REPORT ==========
I (32060) wifi weather station: City: Bengaluru, IN
I (32060) wifi weather station: Local Time: 2025-08-20 17:13:57 (UTC+5)
I (32060) wifi weather station: Temperature: 25.0 °C
I (32070) wifi weather station: Feels like: 25.2 °C
I (32080) wifi weather station: Humidity: 62%
I (32080) wifi weather station: Pressure: 1008 hPa
I (32090) wifi weather station: Condition: Clouds (overcast clouds)
I (32090) wifi weather station: Wind: 7.1 m/s W (260°)
I (32100) wifi weather station: Visibility: 10.0 km
I (32100) wifi weather station: ====================================
I (32110) wifi weather station: HTTPS Status = 200
I (32120) wifi weather station: Disconnected from server