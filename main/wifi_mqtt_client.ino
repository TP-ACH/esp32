#include "wifi_mqtt_client.h"

char device_id[40];

void callback(char *topic_recieved, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic_recieved);
    Serial.print("Message: ");
    // message should be in the format "client_id" for enable/disable topics and i dont read the rest
    String message;
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        message += (char)payload[i];
    }
 
    Serial.println();
    Serial.println("--------");
    int delimiterIndex = String(topic_recieved).indexOf("/");
    String topic_device_id = String(topic_recieved).substring(0, delimiterIndex);

    const char *topic = String(topic_recieved).substring(delimiterIndex + 1).c_str();

    if (topic_device_id != device_id) {
        Serial.println("Message is not for this device");
        return;
    }

    if (!isTopicEnabled(from(topic))) {
        Serial.println("Topic is disabled");
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
            update_topic_status(message, true);
            break;
        }
        case TOPIC_DISABLE: {
            update_topic_status(message, false);
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
        char topic[100];
        snprintf(topic, 100, "%s/%s", device_id, topics_to_sub[i]);
        Serial.println(topic);
        wifiManager.client->subscribe(topic);
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
    snprintf(msg, 200, "{\n\"reading\": \"%f\"\n}", value);
    char topic_with_device_id[100];
    snprintf(topic_with_device_id, 100, "%s/%s", device_id, topic);
    char print_msg[400];
    snprintf(print_msg, 100, "--- publishing message in topic %s ---", topic_with_device_id);
    Serial.println(print_msg);
    Serial.println(msg);
    (*wmm).client->publish(topic_with_device_id, msg, 1);
}
