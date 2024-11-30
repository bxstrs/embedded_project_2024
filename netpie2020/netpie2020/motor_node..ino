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
#define MOTOR_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    if (client.connect(app_id, key, secret)) {
      client.subscribe("@msg/motor_control");
      Serial.println("Connected to NETPIE!");
    } else {
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  if (command == "close") {
    digitalWrite(MOTOR_PIN, HIGH); // Close curtain
  } else if (command == "open") {
    digitalWrite(MOTOR_PIN, LOW); // Open curtain
  }
}

void loop() {
  client.loop();
}
