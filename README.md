# Smart Plant Care Alert System (IoT)

A simulated IoT environmental monitoring and early-warning system built on ESP32, integrating three real-time environmental sensors and publishing dynamic health telemetry over MQTT.

---

## 🔗 Project Links
- **Live Wokwi Simulation:** [https://wokwi.com/projects/472856693493481473]

---

## 📌 Problem Statement
Indoor and agricultural plants frequently suffer from delayed intervention—either due to underwatering, prolonged inadequate sunlight, or undetected waterlogging leading to root rot. This project builds a low-latency, automated edge-monitoring solution that captures microclimate conditions and broadcasts actionable diagnostic alerts.

---

## 🛠️ Components & Architecture
- **Microcontroller:** ESP32 DevKit C v4
- **Sensors:**
  - **DHT22:** Ambient Temperature (°C) & Relative Humidity (%)
  - **LDR Module:** Light Intensity (Simulated Analog / Lux Conversion)
  - **Potentiometer:** Soil Moisture Level (Simulated 0–100%)
- **Connectivity:** Virtual Wi-Fi (`Wokwi-GUEST`) + MQTT Protocol (`broker.hivemq.com:1883`)

---

## 📊 Observed Sensor Ranges & Threshold Reasoning

| Parameter | Observed Simulated Range | Threshold Set | Rationale |
| :--- | :--- | :--- | :--- |
| **Soil Moisture** | 0 – 4095 ADC (0 – 100%) | `< 30%` | Values below 30% indicate parched soil requiring irrigation. |
| **Light Level** | 0 – 4095 ADC (0 – 100%) | `< 20%` | Sustained readings below 20% represent inadequate light for photosynthesis. |
| **Temperature** | -40°C to 80°C | `> 35°C` | High temperatures combined with typical indoor humidity induce thermal stress. |
| **Humidity** | 0 – 100% RH | `> 70%` | High relative humidity combined with excessive moisture accelerates fungal spread. |

---

## 💡 Custom Condition: Fungal & Root Rot Risk Alert
Beyond individual sensor thresholds, this system implements a **multi-variable stress detection algorithm**:

- **Trigger Condition:** `Soil Moisture > 75%` AND `Light Level < 25%` AND `Relative Humidity > 70%`
- **Why It Matters:** Overwatering in low-light, high-humidity environments prevents soil evaporation and root oxygenation, creating an ideal breeding ground for fungal pathogens and root rot. Detecting this composite state allows early drainage intervention before permanent plant damage occurs.

---

## 📡 MQTT Telemetry Payload Structure
Data is broadcast as structured JSON payloads to `vitc/mic/plant_monitor/status`:

```json
{
  "temp": 28.5,
  "humidity": 78.0,
  "soil_moisture": 82,
  "light": 15,
  "status": "ALERT: Fungal & Root Rot Risk (High Moisture + Low Light)"
}
