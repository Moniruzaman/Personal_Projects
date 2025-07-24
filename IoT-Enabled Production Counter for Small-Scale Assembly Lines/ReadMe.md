# IoT-Enabled Production Counter for Small-Scale Assembly Lines

This project is a Wi-Fi-enabled production counter built using a dual-core ESP32 microcontroller. It tracks item counts in small-scale assembly processes through a mechanical limit switch and logs the data in real-time to Firebase. A built-in TFT display shows live production counts and system status, enabling both local and remote monitoring. The device is powered via a rechargeable **Li-ion battery**, ensuring portability and ease of deployment in off-grid or mobile setups.

---

## 📌 Features

- ✅ Real-time counting with mechanical limit switch
- 📶 Wi-Fi connectivity to Firebase Realtime Database
- 📊 Onboard TFT display for count and system status
- 🔋 Powered by a rechargeable Li-ion battery
- ⚙️ Dual-core ESP32 utilization for better task separation
- 🔄 Debounce handling for accurate count detection
- 🔌 Simple, compact, and scalable system for workshops

---

## 🔧 Hardware Specifications

| Component              | Description                                                  |
|------------------------|--------------------------------------------------------------|
| **Microcontroller**    | ESP32 development board (dual-core, with integrated Wi-Fi)   |
| **Display**            | 1.14" IPS TFT LCD (240x135 pixels, SPI Interface)            |
| **Sensor**             | Mechanical Limit Switch (Interrupt trigger on RISING edge)   |
| **Connectivity**       | Wi-Fi (2.4 GHz)                                               |
| **Power Supply**       | Rechargeable 3.7V Li-ion Battery (with onboard charging circuit) or 5V USB-C |

> 💡 The board supports onboard Li-ion battery charging and protection circuitry, allowing continuous operation and safe recharging.

---

## 📡 Cloud Platform

- **Firebase Realtime Database**
  - Stores live production count
  - Enables remote access to production statistics

---

## 🔌 Pin Configuration

| Peripheral         | ESP32 GPIO Pin |
|--------------------|----------------|
| TFT Display (SPI)  | MOSI, SCK, DC, CS, RST (preconfigured) |
| Limit Switch       | GPIO 0         |

> 📌 *Note: GPIO assignments for the TFT display are specific to the onboard hardware and do not require external configuration.*

---

## 🧠 Software Architecture

- **Core 0**: Handles Firebase operations and TFT display refresh
- **Core 1**: Handles limit switch interrupts and production count logic

---

## 🚀 Getting Started

### Prerequisites

- Arduino IDE with ESP32 board support
- Installed Libraries:
  - `TFT_eSPI`
  - `WiFi`
  - `Firebase_ESP_Client`
  - `Wire`


---

## 🔋 Power Options

- **Primary**: 3.7V Li-ion battery (rechargeable, onboard charging supported)
- **Optional**: USB-C / 5V regulated power supply

---
