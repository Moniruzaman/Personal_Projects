# IoT-Based 8-Channel Temperature Data Logger

This project is a robust and scalable **IoT-enabled temperature data logging system** capable of measuring and logging temperature from up to 8 thermocouples using the **MAX31856** sensors and an ESP32-based development board with GSM capability. The system provides real-time display via an I2C 16x2 LCD and is ideal for industrial, laboratory, and remote environmental monitoring.

## 🚀 Features

- Supports **8 thermocouple inputs** using MAX31856 (SPI)
- Real-time temperature monitoring
- Status and fault indication using **LEDs**
- **I2C 16x2 LCD** for local data display
- **GSM (SIM800L)** connectivity (on-board via ESP32 T-CALL)
- Button interface for interaction/reset
- Compact and power-efficient design

## 🧰 Hardware Components

| Component                        | Quantity | Description                                      |
|----------------------------------|----------|--------------------------------------------------|
| ESP32 T-CALL (SIM800L onboard)   | 1        | Microcontroller with Wi-Fi + GSM                |
| MAX31856                         | 8        | Thermocouple-to-digital converter (SPI)         |
| Type-K Thermocouples             | 8        | For temperature measurement                     |
| 16x2 LCD with I2C backpack       | 1        | Display interface                               |
| LEDs (DRDY, FAULT for each sensor) | 16     | Data ready and fault indicators                 |
| Push Button                      | 1        | Manual operation/reset                          |
| 3.3V & 5V Power Supply           | 1        | For powering MCU, sensors, and LCD              |

## 🔌 Pin Configuration

| Peripheral        | ESP32 GPIO Pins    |
|-------------------|--------------------|
| MAX31856 (MOSI)   | GPIO41             |
| MAX31856 (MISO)   | GPIO35             |
| MAX31856 (SCK)    | GPIO34             |
| MAX31856 (CS1–CS8)| GPIO7, 8, 9, 33, 28, 12, 13, 15 |
| MAX31856 (DRDYx)  | Mapped individually |
| MAX31856 (FAULTx) | Mapped individually |
| LCD (SDA/SCL)     | GPIO37 / GPIO40    |
| GSM (UART TX/RX)  | GPIO30 / GPIO3     |
| Button Input      | GPIO4              |

> _Refer to the schematic for precise hardware interconnections._

## 📦 Code Overview

The Arduino sketch performs the following:

- Initializes all SPI-connected MAX31856 sensors
- Reads temperature from each channel sequentially
- Displays readings and status on the LCD
- Detects faults and triggers corresponding LEDs
- Can be extended to send readings over GSM or log to server



