#include <WiFi.h>
#include <WebServer.h>

// Replace these with your network credentials
const char* ssid = "iPhone (8)";
const char* password = "0621491559";

// Set GPIO pin for relay
const int ledPin = 2; // Use onboard LED -> อันนี้ต้องแก้ทีหลัง ถ้าเปลี่ยนชื่อก็แก้ด้านล่างด้วย

WebServer server(80);
bool voiceControlEnabled = false;

// HTML page to serve
const char htmlPage[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Curtain</title>
    <style>
        .toggle-switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }

        .toggle-switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
        }

        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
        }

        input:checked + .slider {
            background-color: #2196F3;
        }

        input:checked + .slider:before {
            transform: translateX(26px);
        }

        .slider.round {
            border-radius: 34px;
        }

        .slider.round:before {
            border-radius: 50%;
        }
    </style>
</head>
<body>
    <h2>Smart Curtain</h2>
    
    <label class="toggle-switch">
        <input type="checkbox" id="toggleOnOff">
        <span class="slider round"></span>
    </label>
    <span>Open/Close Curtain</span>

    <br><br>

    <label class="toggle-switch">
        <input type="checkbox" id="toggleVoiceCommand">
        <span class="slider round"></span>
    </label>
    <span>Enable Voice Command</span>

    <script>
        document.getElementById('toggleOnOff').addEventListener('change', function() {
            if (this.checked) {
                fetch('/open');
            } else {
                fetch('/close');
            }
        });
        
        document.getElementById('toggleVoiceCommand').addEventListener('change', function() {
            if (this.checked) {
                fetch('/enable_voice');
            } else {
                fetch('/disable_voice');
            }
        });
    </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleOpen() {
  digitalWrite(ledPin, HIGH);  // Turn the relay on (Open Curtain)
  server.send(200, "text/plain", "Curtain opened");
}

void handleClose() {
  digitalWrite(ledPin, LOW);  // Turn the relay off (Close Curtain)
  server.send(200, "text/plain", "Curtain closed");
}

void handleEnableVoice() {
  voiceControlEnabled = true;
  Serial.println("Voice control enabled.");
  server.send(200, "text/plain", "Voice control enabled");
}

void handleDisableVoice() {
  voiceControlEnabled = false;
  Serial.println("Voice control disabled.");
  server.send(200, "text/plain", "Voice control disabled");
}

void handleVoiceStatus() {
  server.send(200, "text/plain", voiceControlEnabled ? "true" : "false");
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.on("/close", handleClose);
  server.on("/enable_voice", handleEnableVoice);
  server.on("/disable_voice", handleDisableVoice);
  server.on("/voice_status", handleVoiceStatus);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
