#include "wifi_client.h"

MyWiFiClient::MyWiFiClient(WiFiClient *espClient) : wifi(espClient) {
}

void MyWiFiClient::setup_wifi() {
    pinMode(pin22, OUTPUT);
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(true);

    // Uncomment to reset settings
    // wifiManager.resetSettings();

    // Set timeout to 180 seconds (3 minutes)
    wifiManager.setTimeout(180);

    // Attempt to connect to WiFi
    if (!wifiManager.autoConnect("ESP32_AP")) {
        digitalWrite(pin22, HIGH);
        Serial.println("Failed to connect and hit timeout");
        ESP.restart();
        delay(1000);
        digitalWrite(pin22, LOW);
    }

    digitalWrite(pin22, HIGH);
    Serial.println();
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    delay(200);
    digitalWrite(pin22, LOW);
}