# 🌿 Smart Greenhouse Automation System

An intelligent Arduino-based **Smart Greenhouse System** that automatically monitors and controls key environmental factors — temperature, humidity, rain detection, soil moisture, UV index, and ambient light — to help maintain optimal growing conditions for plants.

## 🧠 Overview

This project integrates multiple sensors and actuators to create an automated greenhouse environment. The system:
- Reads **temperature** and **humidity** using a DHT22 sensor.
- Detects **rain** using a raindrop sensor and responds with LED + buzzer alerts.
- Monitors **soil moisture** to control an automatic water pump.
- Measures **ambient light** with an LDR sensor and switches on an LED light when needed.
- Reads **UV radiation** levels with a UV sensor for sunlight intensity monitoring.
- Operates a **fan** when the temperature exceeds a threshold.
- Uses a **servo motor** to perform actions (e.g., open/close vents or shades) based on soil dryness.
- Displays all readings on a 16x2 I2C LCD and logs data via Serial Monitor.

---

## ⚙️ Hardware Components

| Component | Function |
|------------|-----------|
| Arduino Uno / Mega | Main controller |
| DHT22 Sensor | Temperature & humidity monitoring |
| Raindrop Sensor | Detects rainfall intensity |
| Soil Moisture Sensor | Controls irrigation system |
| LDR Sensor | Measures ambient light intensity |
| UV Sensor (e.g., GUVA-S12SD) | Detects UV index |
| 16x2 I2C LCD | Displays sensor readings |
| Servo Motor | Controls shading or ventilation system |
| 2-Channel Relay Module | Controls fan and water pump |
| Buzzer & LEDs | Alerts for rain and low light |
| Push Button | Toggles system ON/OFF |

---

## 🔌 Pin Connections

| Device | Arduino Pin |
|---------|--------------|
| DHT22 | D2 |
| Button | D3 |
| Fan Relay | D7 |
| Pump Relay | D8 |
| Servo Motor | D9 |
| Buzzer | D11 |
| Rain Sensor | A0 |
| Soil Moisture Sensor | A1 |
| LDR Sensor | A2 |
| UV Sensor | A3 |
| Rain LED | D13 |
| LDR LED | D12 |
| I2C LCD | SDA (A4), SCL (A5) |

---

## 🧩 Libraries Required

Install these libraries via **Arduino Library Manager**:

- `SimpleDHT` (for DHT22)
- `LiquidCrystal_I2C` (for LCD Display)
- `Servo` (for servo motor control)

---

## 🧪 Features

- 🌡️ **Automatic Temperature Control** — turns fan ON if temp > 31.5°C  
- 💧 **Smart Irrigation** — pumps water when soil moisture is below threshold  
- ☔ **Rain Detection** — buzzer + LED alert when rain is detected  
- ☀️ **UV and Light Monitoring** — measures UV index and LDR level  
- 🪄 **Servo Control** — responds to soil dryness by operating vents/shades  
- 🖥️ **LCD Display** — shows live UV voltage, index, and LDR readings  
- 🔘 **Toggle Button** — start or stop system manually  
- 🧰 **Fail-Safe Mode** — sensors turn off and relays reset when stopped  

---

## 🧾 How It Works

1. Press the **button** to activate the system. The LCD shows “Starting Smart Greenhouse.”  
2. The Arduino reads from all sensors and displays values on the **Serial Monitor** and **LCD**.  
3. Based on sensor readings:
   - Fan relay triggers if temperature exceeds the threshold.  
   - Pump relay activates when soil is too dry.  
   - Rain triggers buzzer + rain LED.  
   - LDR controls a light LED.  
4. The system continues monitoring every second (1s loop delay).  
5. Press the button again to stop — all outputs return to safe states.

---

## 🔧 Calibration Tips

- Adjust thresholds as needed:
  - `rainThreshold = 500`
  - `soilThreshold = 650`
  - `SOIL_WET = 210`, `SOIL_DRY = 510`
- Test each sensor individually to find optimal trigger points.
- Use Serial Monitor to log and verify sensor values.

---

## 🧍‍♂️ Usage

1. Upload this sketch to your Arduino.  
2. Connect all components according to the pin map.  
3. Power your setup (via USB or external supply).  
4. Observe live data on the LCD and Serial Monitor.  
5. Adjust sensor thresholds if needed for your environment.

---

## 📸 Optional Enhancements

- Add Wi-Fi (ESP8266/ESP32) for IoT data logging.
- Integrate a real-time clock (RTC) for time-based automation.
- Add an SD card module to store daily temperature and humidity logs.
- Implement a mobile app interface for remote monitoring.

---

## 👨‍💻 Author

**Project Developer:** [Your Name]  
**University:** [Your Institution, e.g., BRAC University]  
**Department:** Computer Science and Engineering  
**Language:** Arduino C++  
**Version:** 1.0  

---

## 🪪 License

This project is open-source and released under the [MIT License](https://opensource.org/licenses/MIT). You may modify or redistribute it freely with proper attribution.

---

