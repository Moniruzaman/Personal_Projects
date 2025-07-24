# 📇 RFID Attendance Device (ESP8266 + RDM6300)

A Wi-Fi-enabled RFID Attendance System built using the ESP8266 (e.g., Wemos D1 Mini) and the RDM6300 125KHz RFID reader. This project uses **WiFiManager** for easy Wi-Fi configuration and sends scanned RFID tag data to a remote server for authentication and logging.

---

## 🚀 Features

- 📶 **WiFiManager Integration**  
  Dynamic access point for Wi-Fi configuration without hardcoding credentials.

- 📡 **RFID Tag Scanning**  
  Uses RDM6300 module to read 125KHz RFID tags (e.g., EM4100, TK4001).

- ☁️ **Remote Authentication**  
  Sends tag data via HTTP GET request to a remote server for verification.

- 🔊 **Buzzer and Relay Output**  
  Provides audio-visual feedback and relay activation on successful access.

- 🔘 **Physical Button Input**  (Optional)
  Manual override button allows relay triggering without RFID.

- ♻️ **Wi-Fi Reset Button**  
  Hold `D8` for 3 seconds to erase WiFi credentials and reconfigure.

---

## 🧾 Technical Specifications

| Component         | Specification                          |
|------------------|------------------------------------------|
| Microcontroller   | ESP8266 (e.g., Wemos D1 Mini)           |
| RFID Reader       | RDM6300 (125KHz)                        |
| Communication     | Wi-Fi (802.11 b/g/n)                    |
| Operating Voltage | 5V for RFID, 3.3V logic for ESP8266     |
| Data Sync         | HTTP GET API call                       |
| Output            | Buzzer, Relay (Optional)                          |
| API Endpoint      | `http://sns.orientcomputers.com/api/device-data` |
| Input             | Manual Access Button, WiFi Reset Button |

---

## 📌 Pin Mapping

| Device         | ESP8266 GPIO | Description                     |
|----------------|--------------|---------------------------------|
| RDM6300 RX     | GPIO14 (D5)  | Serial input from RFID module   |
| Buzzer         | GPIO12 (D6)  | Audio feedback on access        |
| Relay          | GPIO5  (D1)  | Triggered on successful scan (Optional)   |
| Manual Button  | GPIO4  (D2)  | Triggers relay+beep manually    |
| Reset Button   | GPIO15 (D8)  | Hold to reset WiFi credentials  |
| Status LED     | GPIO2  (D4)  | Visual status indication        |

---

## 🌐 Server API Format

Sends an HTTP GET request like:
```
http://sns.orientcomputers.com/api/device-data?macid=12909&rfid=TAG_ID
```

### Example Response (JSON):
```json
{
  "access": 1
}
```

- `access = 1` → Access granted (relay ON)
- `access = 0` → Access denied (buzzer only)

---

## 🛠️ Libraries Used

- [`WiFiManager`](https://github.com/tzapu/WiFiManager)
- `ESP8266WiFi.h`
- `ESP8266HTTPClient.h`
- `SoftwareSerial.h`
- `Arduino_JSON.h`
- Custom: `rdm6300.h`

---

## 🖥️ Setup Instructions

1. **Install Required Libraries** via Library Manager or manually.
2. **Connect your ESP8266** with the RDM6300, buzzer, relay, and buttons.
3. **Upload the Code** using Arduino IDE.
4. On first boot, connect to the AP named `AutoConnectAP` and configure WiFi.
5. Scan RFID cards or press the button to trigger access.

---

## 📷 Schematic

The complete circuit is available in the [schematics folder](schematics/Schematic_RFID_Test_2023-10-16.pdf).

---

## 👨‍💻 Developed By

**Md Moniruzaman**  
Manager, R&D  
[AurexIoT Systems](https://github.com/Moniruzaman)

---

## 📄 License

This project is open-source under the [MIT License](LICENSE).
