#ifndef TOPIC_H
#define TOPIC_H

#include <string.h>

// topic_enum
enum Topic {
    TOPIC_LIGHT_ON,
    TOPIC_LIGHT_OFF,
    TOPIC_ACID,
    TOPIC_BASE,
    TOPIC_WATER,
    TOPIC_NUTES,
    TOPIC_TEMPERATURE,
    TOPIC_PH,
    TOPIC_EC,
    TOPIC_HUMIDITY,
    TOPIC_ROOM_TEMPERATURE,
    TOPIC_FLOATER,
    TOPIC_COUNT // This will be the number of topics
};

const char* getTopicString(Topic topic);

Topic from(const char* topic);

#endif