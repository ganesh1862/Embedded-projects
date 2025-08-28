# 📁 ESP32 SPIFFS Demo

This project demonstrates how to initialize and use the SPIFFS (SPI Flash File System) on an ESP32 using ESP-IDF. It mounts the filesystem, writes a simple text file, and reads it back to verify functionality.

## 🚀 Features

- Mounts SPIFFS partition at `/spiffs`
- Automatically formats if mount fails
- Writes `"Hello Gani!\n"` to `hello.txt`
- Reads and logs the file contents
- Uses FreeRTOS and ESP-IDF v5.3.1

## 🧰 Requirements

- ESP32 board with ≥2MB flash (16MB recommended)
- ESP-IDF v5.3.1 or later
- Custom partition table with SPIFFS partition
- Flash size set correctly in `menuconfig`

## 📦 Partition Table Example (`partitions.csv`)

```csv
# Name,     Type, SubType, Offset,   Size,     Flags
nvs,        data, nvs,     0x9000,   0x6000,
phy_init,   data, phy,     0xF000,   0x1000,
factory,    app,  factory, 0x10000,  1M,
spiffs,     data, spiffs,  0x110000, 1M,
