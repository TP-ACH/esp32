#include "topic.h"

// Define an array of strings corresponding to the topics
const char* all_topics[TOPIC_COUNT] = {
    "actuators/light_on",
    "actuators/light_off",
    "actuators/pumps/ph_down",
    "actuators/pumps/ph_up",
    "actuators/pumps/water",
    "actuators/pumps/nutrient",
    "sensors/ph",
    "sensors/ph_voltage",
    "sensors/ec",
    "sensors/humidity",
    "sensors/temperature",
    "sensors/floater",
    "sensors/enable",
    "sensors/disable"
};

std::map<Topic, Status> topic_map = {
    {TOPIC_LIGHT_ON, ON},
    {TOPIC_LIGHT_OFF, ON},
    {TOPIC_ACID, ON},
    {TOPIC_BASE, ON},
    {TOPIC_WATER, ON},
    {TOPIC_NUTES, ON},
    {TOPIC_PH, ON},
    {TOPIC_PH_VOLTAGE, ON},
    {TOPIC_EC, ON},
    {TOPIC_HUMIDITY, ON},
    {TOPIC_TEMPERATURE, ON},
    {TOPIC_FLOATER, ON},
    {TOPIC_ENABLE, ON},
    {TOPIC_DISABLE, ON},
    {TOPIC_COUNT, OFF} // to ignore invalid ones
};

const char* getTopicString(Topic topic) {
    if (topic >= 0 && topic < TOPIC_COUNT) {
        Serial.print("Topic: ");
        Serial.println(all_topics[topic]);
        return all_topics[topic];
    }
    return "";
}

Topic from(const char* topic) {
    for (int i = 0; i < TOPIC_COUNT; ++i) {
        if (strcmp(all_topics[i], topic) == 0) {
            return static_cast<Topic>(i);
        }
    }
    return TOPIC_COUNT; // never gets here
}

void update_topic_status(String topic, bool status) {
    Serial.print("Validating topic: ");
    Topic topic_enum = from(topic.c_str());
    if (topic_enum == TOPIC_COUNT) {
        Serial.println("Invalid topic");
        return;
    }
    if (topic_enum == TOPIC_ENABLE || topic_enum == TOPIC_DISABLE) {
        Serial.println("Cannot enable or disable the enable or disable topic");
        return;
    }
    Serial.print("Updating topic: ");
    Serial.println(topic);
    Serial.print("Status: ");
    Serial.println(status);
    topic_map[topic_enum] = status ? ON : OFF;
}

bool isTopicEnabled(Topic topic) {
    return topic_map[topic] == ON;
}