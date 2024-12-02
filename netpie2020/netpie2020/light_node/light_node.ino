#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// NETPIE Configuration
const char* ssid = "Sleep with me free wifi_2.4G";
const char* password = "0814782645";
const char* mqtt_server = "broker.netpie.io";
const char* app_id = "7981b3de-d928-4fad-9acf-47b5c2ffcc5f";
const char* key = "btCmhwRxcNTRWe5fCExj49dSad4CQXzP";
const char* secret = "6UoyGpzWAD7TxFocNzT4P4Fx9H6bziKz";

WiFiClient espClient;
PubSubClient client(espClient);
// LED Pin
#define LED_PIN 5

// Variables for sensor data
float light = 0.0, temp = 0.0, humid = 0.0; // Variables to store sensor data

void setup() {
  Serial.begin(115200);
  
  // Set LED_PIN as output
  pinMode(LED_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Connect to MQTT server
  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    if (client.connect(app_id, key, secret)) {
      client.subscribe("@msg/led_control");  // Subscribe to LED control messages
      client.subscribe("@msg/sensor_data");  // Subscribe to sensor data messages
      Serial.println("Connected to NETPIE!");
    } else {
      delay(5000);
    }
  }
}

// Callback function to handle incoming MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Handle LED control commands
  if (String(topic) == "@msg/led_control") {
    if (message == "on") {
      digitalWrite(LED_PIN, HIGH);  // Turn the LED ON
      Serial.println("LED ON");
    } else if (message == "off") {
      digitalWrite(LED_PIN, LOW);   // Turn the LED OFF
      Serial.println("LED OFF");
    }
  }

  // Handle sensor data
  if (String(topic) == "@msg/sensor_data") {
    // Parse sensor data (assuming it's sent in JSON format)
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (!error) {
      light = doc["light"];   // Extract light level
      temp = doc["temp"];     // Extract temperature
      humid = doc["humid"];   // Extract humidity

      Serial.printf("Sensor Data - Light: %.2f, Temp: %.2f, Humidity: %.2f\n", light, temp, humid);

      // Decide to turn the LED on or off based on conditions
      if (light <= 50 || humid >= 60.0) {
        digitalWrite(LED_PIN, HIGH);  // Turn LED on
        Serial.println("LED ON based on sensor data");
      } else if (light > 50 || humid < 60.0) {
        digitalWrite(LED_PIN, LOW);   // Turn LED off
        Serial.println("LED OFF based on sensor data");
      }
    } else {
      Serial.println("Failed to parse sensor data");
    }
  }
}

void loop() {
  client.loop();
}
