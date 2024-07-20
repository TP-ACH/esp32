#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include "constants.h"


class MQTTClient {
public:
    // Constructor
    MQTTClient(PubSubClient client, String mac_address);

    // Public methods
    void publish(const char* topic, int value);
    void reconnect(String mac_address);

private:
    void setup_mqtt(PubSubClient client, String mac_address);
    PubSubClient client;
    String client_id;
};

#endif 