#include "wifi_mqtt_client.h"

void callback(char *topic, byte *payload, unsigned int length) {
    digitalWrite(pin22, HIGH);
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("--------");
    digitalWrite(pin22, LOW);
    
    switch (from(topic)) {
        case TOPIC_WATER: {
            digitalWrite(p_water, HIGH);
            delay(3000);
            digitalWrite(p_water, LOW);
            break;
        }
        default: {}
    }
}

WiFiAndMQTTClient::WiFiAndMQTTClient() {
}

WiFiMQTTManager wifiManager(p_reset, ap_password);


void subcribe() {
    for (int i = 0; i < sizeof(topics_to_sub)/sizeof(topics_to_sub[0]); i++) {
        Serial.print("Subscribing to topic: ");
        Serial.println(topics_to_sub[i]);
        wifiManager.client->subscribe(topics_to_sub[i]);
    }
}

void WiFiAndMQTTClient::setup() {
    
    wmm = &wifiManager;
    wifiManager.setDebugOutput(true);

    // Attempt to connect to WiFi
    wifiManager.setup();
    client_id = wifiManager.clientId;
    
    wifiManager.subscribeTo = subcribe;
    wifiManager.client -> setCallback(callback);

    // if (!wifiManager.autoConnect("ESP32_AP")) {
    //     digitalWrite(pin22, HIGH);
    //     Serial.println("Failed to connect and hit timeout");
    //     ESP.restart();
    //     delay(1000);
    //     digitalWrite(pin22, LOW);
    // }

    // digitalWrite(pin22, HIGH);
    // Serial.println();
    // Serial.println("Connected to WiFi");
    // Serial.print("IP Address: ");
    // Serial.println(WiFi.localIP());
    // delay(200);
    // digitalWrite(pin22, LOW);
}

void WiFiAndMQTTClient::loop() {
    (*wmm).loop();
}

void WiFiAndMQTTClient::publish(const char* topic, float value) {
    char msg[200];
    snprintf(msg, 200, "{\n\"device_id\": \"%s\", \n\"reading\": \"%f\"\n}", client_id.c_str(), value);
    char print_msg[100];
    snprintf(print_msg, 100, "--- publishing message in topic %s ---", topic);
    Serial.println(print_msg);
    Serial.println(msg);
    (*wmm).client->publish(topic, msg, 1);
}
