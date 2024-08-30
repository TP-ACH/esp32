#ifndef WIFI_MQTT_CLIENT_H
#define WIFI_MQTT_CLIENT_H

#include "WiFiMQTTManager.h"
#include "topic.h"

class WiFiAndMQTTClient {
public:
    WiFiAndMQTTClient();
    void setup();
    void publish(const char* topic, float value);
    void loop();

private:
    WiFiMQTTManager *wmm;
    String client_id;
};

#endif // WIFI_MQTT_CLIENT_H