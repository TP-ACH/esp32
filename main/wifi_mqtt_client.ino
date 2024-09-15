#include "wifi_mqtt_client.h"

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("--------");

    switch (from(topic)) {
        case TOPIC_WATER: {
            digitalWrite(p_water, HIGH);
            delay(3000);
            digitalWrite(p_water, LOW);
            break;
        }
        case TOPIC_NUTES: {
            digitalWrite(p_nutes, HIGH);
            delay(3000);
            digitalWrite(p_nutes, LOW);
            break;
        }
        case TOPIC_ACID: {
            digitalWrite(p_acid, HIGH);
            delay(200);
            digitalWrite(p_acid, LOW);
            break;
        }
        case TOPIC_BASE: {
            digitalWrite(p_base, HIGH);
            delay(200);
            digitalWrite(p_base, LOW);
            break;
        }
        case TOPIC_LIGHT_ON: {
            digitalWrite(p_lights, HIGH);
            break;
        }
        case TOPIC_LIGHT_OFF: {
            digitalWrite(p_lights, LOW);
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
