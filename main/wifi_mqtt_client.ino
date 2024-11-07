#include "wifi_mqtt_client.h"


struct DeviceState {
    bool isOn;
    unsigned long startTime;
    unsigned long duration;
};

DeviceState waterPump = {false, 0, 4000};  // 4 seconds for water
DeviceState nutesPump = {false, 0, 4000};   // 4 seconds for nutrients
DeviceState acidPump = {false, 0, 4000};   // 4 seconds for acid
DeviceState basePump = {false, 0, 4000};   // 4 seconds for base

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
            waterPump.isOn = true;
            waterPump.startTime = millis();
            break;
        }
        case TOPIC_NUTES: {
            digitalWrite(p_nutes, HIGH);
            nutesPump.isOn = true;
            nutesPump.startTime = millis();
            break;
        }
        case TOPIC_ACID: {
            digitalWrite(p_acid, HIGH);
            acidPump.isOn = true;
            acidPump.startTime = millis();
            break;
        }
        case TOPIC_BASE: {
            digitalWrite(p_base, HIGH);
            basePump.isOn = true;
            basePump.startTime = millis();
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
        wifiManager.client->subscribe(topic, 1);
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


void loop_callback() {
     unsigned long currentMillis = millis();

    if (waterPump.isOn && currentMillis - waterPump.startTime >= waterPump.duration) {
        digitalWrite(p_water, LOW);
        waterPump.isOn = false;
    }

    if (nutesPump.isOn && currentMillis - nutesPump.startTime >= nutesPump.duration) {
        digitalWrite(p_nutes, LOW);
        nutesPump.isOn = false;
    }

    if (acidPump.isOn && currentMillis - acidPump.startTime >= acidPump.duration) {
        digitalWrite(p_acid, LOW);
        acidPump.isOn = false;
    }

    if (basePump.isOn && currentMillis - basePump.startTime >= basePump.duration) {
        digitalWrite(p_base, LOW);
        basePump.isOn = false;
    }
}


void WiFiAndMQTTClient::loop() {
    loop_callback();
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
    (*wmm).client->publish(topic_with_device_id, msg);
}
