#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

// NETPIE Configuration
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
const char* mqtt_server = "mqtt.netpie.io";
const char* app_id = "Your_APP_ID";
const char* key = "Your_KEY";
const char* secret = "Your_SECRET";

WiFiClient espClient;
PubSubClient client(espClient);

BH1750 lightMeter;
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin();
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    if (client.connect(app_id, key, secret)) {
      Serial.println("Connected to NETPIE!");
    } else {
      delay(5000);
    }
  }
}

void loop() {
  float lightLevel = lightMeter.readLightLevel();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!client.connected()) {
    client.connect(app_id, key, secret);
  }

  String data = String("{\"light\":") + lightLevel +
                ",\"temperature\":" + temperature +
                ",\"humidity\":" + humidity + "}";
  client.publish("@msg/sensor_data", data.c_str());

  delay(5000);
}
