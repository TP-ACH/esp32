#include "mqtt_client.h"

MQTTClient::MQTTClient(PubSubClient pubsub) : client(pubsub) {
}

void MQTTClient::reconnect(String mac_address) {
    while (!client.connected()) {
        digitalWrite(pin22, HIGH);
        client_id = "esp32-client-";
        client_id += mac_address;

        Serial.printf("The client %s connects to the MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("MQTT broker connected");
            digitalWrite(pin22, LOW);
        } else {
            Serial.print("Failed with state ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

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

void MQTTClient::setup_mqtt(String mac_address) {
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    MQTTClient::reconnect(mac_address);

    for (int i = 0; i < sizeof(topics_to_sub)/sizeof(topics_to_sub[0]); i++) {
        client.subscribe(topics_to_sub[i]);
    }
}

void MQTTClient::publish(const char* topic, int value) {
    delay(1000);  // Consider replacing this with a non-blocking delay
    char msg[200];
    snprintf(msg, 200, "{\n\"device_id\": \"%s\", \n\"reading\": \"%d\"\n}", client_id.c_str(), value);
    char print_msg[100];
    snprintf(print_msg, 100, "--- publishing message in topic %s ---", topic);
    Serial.println(print_msg);
    Serial.println(msg);
    client.publish(topic, msg, 1);
    delay(1000);  // Consider replacing this with a non-blocking delay
}