#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <WiFiManager.h> 
#include <cleverleafy.h>

#define USE_SERIAL Serial


String client_id = "esp32-client-";

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    digitalWrite(pin22, HIGH);
    client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());

    Serial.printf("The client %s connects to the MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("MQTT broker connected");
      digitalWrite(pin22, LOW);
      
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void input_pins() {
  pinMode(p_temperature, INPUT);
  pinMode(p_room_temperature, INPUT);
  pinMode(p_pH, INPUT);
  pinMode(p_ec, INPUT);
  pinMode(p_humidity, INPUT);
  pinMode(p_floater, INPUT);
}

void output_pins() {
  pinMode(p_acid, OUTPUT);
  pinMode(p_base, OUTPUT);
  pinMode(p_lights, OUTPUT);
  pinMode(p_water, OUTPUT);
  pinMode(p_nutes, OUTPUT);
}

void callback(char *topic, byte *payload, unsigned int length) {
  digitalWrite(pin22, HIGH);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("--------");
  digitalWrite(pin22, LOW);
  
  switch (from(topic)) {
    case TOPIC_WATER: {
      digitalWrite(p_water, HIGH);
      delay(3000);
      digitalWrite(p_water, LOW);
      break;
    }
    default: {}
  }
}

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  pinMode(pin22, OUTPUT);
  wifiManager.setDebugOutput(true);

  // Uncomment to reset settings
  // wifiManager.resetSettings();

  // Set timeout to 180 seconds (3 minutes)
  wifiManager.setTimeout(180);

  if (!wifiManager.autoConnect("ESP32_AP")) {
    digitalWrite(pin22, HIGH);
    Serial.println("Failed to connect and hit timeout");
    ESP.restart();
    delay(1000);
    digitalWrite(pin22, LOW);
  }

  digitalWrite(pin22, HIGH);
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  delay(200);
  digitalWrite(pin22, LOW);

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  reconnect();
  for (int i = 0; i < sizeof(topics_to_sub)/sizeof(topics_to_sub[0]); i++) {
    client.subscribe(topics_to_sub[i]);
  }
  input_pins();
  output_pins();
}

void publish(int value, const char* topic) {
  delay(1000);
  char msg[200];
  snprintf(msg, 200, "{\n\"device_id\": \"%s\", \n\"reading\": \"%d\"\n}", client_id.c_str(), value);
  char print_msg[100];
  snprintf(print_msg, 100, "--- publishing message in topic %s ---", topic);
  Serial.println(print_msg);
  Serial.println(msg);
  client.publish(topic, msg, 1);
  delay(1000);
}

void analog_read() {
  // int temperature = analogRead(p_temperature);
  // publish(temperature, topic_temperature);
  // int room_temperature = analogRead(p_room_temperature);
  // publish(room_temperature, topic_room_temperature);
  // int pH = analogRead(p_pH);
  // publish(pH, topic_ph);
  // int ec = analogRead(p_ec);
  // publish(ec, topic_ec);
  // int humidity = analogRead(p_humidity);
  // publish(humidity, topic_humidity);
  int floater = digitalRead(p_floater);
  publish(floater, getTopicString(TOPIC_FLOATER));
}

unsigned long last_receive_time = 0;
unsigned long last_publish_time = 0;
const unsigned long receive_interval = 10000; // 10 seconds

boolean receiving = true;  // Flag to indicate receiving state

void loop() {
  if (!client.connected()) {
    reconnect();
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
    // Check if it's time to publish a message
    analog_read();
    receiving = true;
    Serial.println("Switching to receiving mode");
  }
  delay(1000);
}