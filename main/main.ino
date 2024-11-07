#include "cleverleafy.h"


WiFiAndMQTTClient wifi_mqtt_client;
GravityTDS tds;
TDSHelper tds_helper(&tds);
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR); //sensor address, sensor type
AHT10Helper aht10_helper(&aht10);
pH4502cHelper ph4502c_helper;

unsigned long last_receive_time = 0;
unsigned long last_publish_time = 0;

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  wifi_mqtt_client.setup();
  Serial.println("wifi and mqtt setup done");
  yield();
  delay(2000); 
  input_pins();
  output_pins();
  Serial.println("pins setup done");
  tds_helper.setup_tds();
  aht10_helper.setup_aht10(p_sda, p_scl);
  ph4502c_helper.init(p_pH);
  Serial.println("setup done");
}

void analog_read() {
  if (isTopicEnabled(TOPIC_PH)) {
    float pH = ph4502c_helper.read_ph_level();
    wifi_mqtt_client.publish(getTopicString(TOPIC_PH), pH);

    float ph_voltage = ph4502c_helper.get_last_voltage_reading();
    wifi_mqtt_client.publish(getTopicString(TOPIC_PH_VOLTAGE), ph_voltage);
  }

  if (isTopicEnabled(TOPIC_TEMPERATURE)) {
    float room_temperature = aht10_helper.read_temperature();
    wifi_mqtt_client.publish(getTopicString(TOPIC_TEMPERATURE), room_temperature);

    if (isTopicEnabled(TOPIC_EC)) {
      float ec = tds_helper.analog_read(room_temperature);
      wifi_mqtt_client.publish(getTopicString(TOPIC_EC), ec);
    }
  } else if (isTopicEnabled(TOPIC_EC)) {
    float ec = tds_helper.analog_read(23);
    wifi_mqtt_client.publish(getTopicString(TOPIC_EC), ec);
  }

  if (isTopicEnabled(TOPIC_HUMIDITY)) {
    float humidity = aht10_helper.read_humidity();
    wifi_mqtt_client.publish(getTopicString(TOPIC_HUMIDITY), humidity);
  }
  
  if (isTopicEnabled(TOPIC_FLOATER)) {
    int floater = digitalRead(p_floater);
    wifi_mqtt_client.publish(getTopicString(TOPIC_FLOATER), floater);
  }
}

void loop() {
  delay(3000); 
  wifi_mqtt_client.loop();
  analog_read();
}