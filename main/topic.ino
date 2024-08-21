#include <constants.h>
#include <cleverleafy.h>
#include <string.h>

// Define an array of strings corresponding to the topics
const char* all_topics[TOPIC_COUNT-1] = {
    "actuators/light",
    "actuators/pumps/ph_down",
    "actuators/pumps/ph_up",
    "actuators/pumps/water",
    "actuators/pumps/nutrient"
    "sensors/temperature",
    "sensors/ph",
    "sensors/ec",
    "sensors/humidity",
    "sensors/room_temperature",
    "sensors/floater"
};

const char* getTopicString(Topic topic) {
    if (topic >= 0 && topic < (TOPIC_COUNT-1)) {
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
    return TOPIC_UNKNOWN;
}