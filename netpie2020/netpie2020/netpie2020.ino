#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "vivo Y03";
const char* password = "sr09062004";

const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "5877eacd-bfab-4110-8678-c1aa1c79ea19";
const char* mqtt_username = "7MdDdMg2i3YXAezbdNjZ2WJApaucDE6N";
const char* mqtt_password = "3eyJvnDF82GBtaLTp36oNTLtVXygk6HE"; 

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting NETPIE2020 connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("NETPIE2020 connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  client.publish("@msg/test", "NETPIE2020 publish");
  //Serial.println("NETPIE2020 publish");
  delay(5000);

}
