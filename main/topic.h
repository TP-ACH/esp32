#ifndef TOPIC_H
#define TOPIC_H


// topic_enum
enum Topic {
    TOPIC_LIGHTS,
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
    TOPIC_UNKNOWN,
    TOPIC_COUNT // This will be the number of topics
};

const char* getTopicString(Topic topic);

Topic from(const char* topic);

#endif