#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>

// NETPIE Configuration
const char* ssid = "Sleep with me free wifi_2.4G";
const char* password = "0814782645";
const char* mqtt_server = "broker.netpie.io";
const char* app_id = "5877eacd-bfab-4110-8678-c1aa1c79ea19";
const char* key = "7MdDdMg2i3YXAezbdNjZ2WJApaucDE6N";
const char* secret = "3eyJvnDF82GBtaLTp36oNTLtVXygk6HE";

WiFiClient espClient;
PubSubClient client(espClient);

BH1750 lightMeter;
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA = GPIO 21, SCL = GPIO 22
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23);
  
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
  // Read light intensity
  float lux = lightMeter.readLightLevel();
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  Serial.print("Light Intensity: ");Serial.print(lux);Serial.println(" lx");
  Serial.print("Temp: "); Serial.print(temp); Serial.println(" °C ");
  Serial.print("Humidity: "); Serial.print(humid); Serial.println(" %");

  
  if (!client.connected()) {
    client.connect(app_id, key, secret);
  }
  

  String data = String("{\"light\":") + lux +
                ",\"temperature\":" + temp +
                ",\"humidity\":" + humid + 
                "}";
  client.publish("@msg/sensor_data", data.c_str());

  delay(5000);
}
