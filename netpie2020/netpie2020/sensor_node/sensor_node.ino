#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>

// NETPIE Configuration
const char* ssid = "Er";
const char* password = "00000000";
const char* mqtt_server = "broker.netpie.io";
const char* app_id = "5877eacd-bfab-4110-8678-c1aa1c79ea19";
const char* key = "7MdDdMg2i3YXAezbdNjZ2WJApaucDE6N";
const char* secret = "3eyJvnDF82GBtaLTp36oNTLtVXygk6HE";

WiFiClient espClient;
PubSubClient client(espClient);

BH1750 lightMeter;
#define DHTPIN 4
#define DHTTYPE DHT22
//DHT dht(DHTPIN, DHTTYPE);

WebServer server(80);

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  >> Webserver here
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", html);
}


void setup() {
  Serial.begin(115200);

  //Light intensity sensor code
  Wire.begin(21, 22); // I2C Communication Pins
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 initialized successfully.");
  } else {
    Serial.println("Error initializing BH1750");
    while (1);
  }

  //dht.begin();
  /*
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
  */
}

void loop() {

  float lux = lightMeter.readLightLevel();
  //float temp = dht.readTemperature();
  //float humid = dht.readHumidity();

  Serial.print("Light Intensity: ");
  Serial.print(lux);
  Serial.print(" lux");
  
  Serial.print("Temperature: ");
  //Serial.println(temp);
  Serial.print("Humidity: ");
  //Serial.println(humid);

  /*
  if (!client.connected()) {
    client.connect(app_id, key, secret);
  }
  

  String data = String("{\"light\":") + //lux +
                //",\"temperature\":" + temperature +
                //",\"humidity\":" + humidity + 
                "}";
  client.publish("@msg/sensor_data", data.c_str());
  */

  delay(5000);
}
