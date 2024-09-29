#ifndef TOPIC_H
#define TOPIC_H

#include <string.h>
#include <map>


// topic_enum
enum Topic {
    TOPIC_LIGHT_ON,
    TOPIC_LIGHT_OFF,
    TOPIC_ACID,
    TOPIC_BASE,
    TOPIC_WATER,
    TOPIC_NUTES,
    TOPIC_PH,
    TOPIC_PH_VOLTAGE,
    TOPIC_EC,
    TOPIC_HUMIDITY,
    TOPIC_ROOM_TEMPERATURE,
    TOPIC_FLOATER,
    TOPIC_ENABLE,
    TOPIC_DISABLE,
    TOPIC_COUNT // This will be the number of topics
};

enum Status {
    ON,
    OFF
};

const char* getTopicString(Topic topic);

Topic from(const char* topic);

void update_topic_status(String topic, bool status);

bool isTopicEnabled(Topic topic);

#endif