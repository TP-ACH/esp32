#include <cleverleafy.h>

WiFiClient espClient;
PubSubClient client(espClient);
MyWiFiClient wifi_client(&espClient);
MQTTClient mqtt_client(&client);

unsigned long last_receive_time = 0;
unsigned long last_publish_time = 0;
const unsigned long receive_interval = 10000; // 10 seconds
boolean receiving = true;  // Flag to indicate receiving state

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  wifi_client.setup_wifi();
  delay(1000);
  mqtt_client.setup_mqtt(String(WiFi.macAddress()));
  input_pins();
  output_pins();
}

void analog_read() {
  // Uncomment and define these lines as needed
  // int temperature = analogRead(p_temperature);
  // mqtt_client.publish(getTopicString(TOPIC_TEMPERATURE), temperature);
  // int room_temperature = analogRead(p_room_temperature);
  // mqtt_client.publish(getTopicString(TOPIC_ROOM_TEMPERATURE), room_temperature);
  // int pH = analogRead(p_pH);
  // mqtt_client.publish(getTopicString(TOPIC_PH), pH);
  // int ec = analogRead(p_ec);
  // mqtt_client.publish(getTopicString(TOPIC_EC), ec);
  // int humidity = analogRead(p_humidity);
  // mqtt_client.publish(getTopicString(TOPIC_HUMIDITY), humidity);
  int floater = digitalRead(p_floater);
  mqtt_client.publish(getTopicString(TOPIC_FLOATER), floater);
}

void loop() {
  if (!client.connected()) {
    mqtt_client.reconnect(String(WiFi.macAddress()));
  }
  client.loop();

  unsigned long currentMillis = millis();

  if (receiving) {
    // Check if it's time to receive messages
    if (currentMillis - last_receive_time >= receive_interval) {
      last_receive_time = currentMillis;
      // Switch to publishing mode after receiving for 10 seconds
      receiving = false;
      Serial.println("Switching to publishing mode");
    }
  } else {
    // Publish sensor readings
    analog_read();
    receiving = true;
    Serial.println("Switching to receiving mode");
  }
}