#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT22
#define LDRPIN 34
#define SOILPIN 35

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* topic = "vitc/mic/plant_monitor/status";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      // Successfully connected to MQTT Broker
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int rawLdr = analogRead(LDRPIN);
  int rawSoil = analogRead(SOILPIN);

  // Convert raw 12-bit ADC (0-4095) to percentage (0-100%)
  float soilPercent = map(rawSoil, 0, 4095, 0, 100);
  float lightPercent = map(rawLdr, 0, 4095, 100, 0);

  String status = "Healthy";

  // Decision Logic + Custom Condition
  if (soilPercent > 75.0 && lightPercent < 25.0 && humidity > 70.0) {
    status = "ALERT: Fungal & Root Rot Risk (High Moisture + Low Light)";
  } else if (soilPercent < 30.0) {
    status = "Needs Water";
  } else if (lightPercent < 20.0) {
    status = "Needs Sunlight";
  } else if (temp > 35.0) {
    status = "Heat Stress Warning";
  }

  // Construct JSON Payload
  String payload = "{\"temp\":" + String(temp, 1) + 
                  ",\"humidity\":" + String(humidity, 1) + 
                  ",\"soil_moisture\":" + String(soilPercent, 0) + 
                  ",\"light\":" + String(lightPercent, 0) + 
                  ",\"status\":\"" + status + "\"}";

  Serial.println(payload);
  client.publish(topic, payload.c_str());

  delay(3000);
}
