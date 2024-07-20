#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include "constants.h"

class MQTTClient {
public:
    MQTTClient(PubSubClient pubsub);
    void reconnect(String mac_address);
    void publish(const char* topic, int value);
    void setup_mqtt(String mac_address);

private:
    PubSubClient client;
    String client_id;
};

#endif // MQTT_CLIENT_H