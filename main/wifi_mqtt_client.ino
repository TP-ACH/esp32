#include "wifi_mqtt_client.h"

char device_id[40];

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    // message should be in the format "topic;client_id" for enable/disable topics and "client_id" for the rest
    String message;
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        message += (char)payload[i];
    }
    Serial.println();
    Serial.println("--------");

    if (!isTopicEnabled(from(topic))) {
        Serial.println("Topic is disabled");
        return;
    }
    String topic_to_modify;
    if (from(topic) == TOPIC_ENABLE || from(topic) == TOPIC_DISABLE) {
        int delimiterIndex = message.indexOf(";");
        topic_to_modify = message.substring(0, delimiterIndex);
        message = message.substring(delimiterIndex + 1);
    }

    if (message != device_id) {
        Serial.println("Message is not for this device");
        return;
    }

    switch (from(topic)) {
        case TOPIC_WATER: {
            digitalWrite(p_water, HIGH);
            delay(200);
            digitalWrite(p_water, LOW);
            break;
        }
        case TOPIC_NUTES: {
            digitalWrite(p_nutes, HIGH);
            delay(200);
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
        case TOPIC_ENABLE: {
            update_topic_status(topic_to_modify, true);
            break;
        }
        case TOPIC_DISABLE: {
            update_topic_status(topic_to_modify, false);
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
    strcpy(device_id, wifiManager.clientId);
    Serial.println("Setting up MQTT client...");
    wifiManager.subscribeTo = subcribe;
    Serial.println("Setting up callback...");
    wifiManager.client -> setCallback(callback);
    Serial.println("Subscribing to topics...");
    subcribe();
    
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
