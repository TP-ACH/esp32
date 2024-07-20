#include <cleverleafy.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiManager.h>

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long last_receive_time = 0;
unsigned long last_publish_time = 0;
const unsigned long receive_interval = 10000; // 10 seconds
boolean receiving = true;  // Flag to indicate receiving state
String client_id = "esp32-client-";

void reconnect(String mac_address) {
    while (!client.connected()) {
        digitalWrite(pin22, HIGH);
        client_id = "esp32-client-";
        client_id += mac_address;

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

void setup_mqtt(String mac_address) {
 pinMode(pin22, OUTPUT);
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(true);

    // Uncomment to reset settings
    // wifiManager.resetSettings();

    // Set timeout to 180 seconds (3 minutes)
    wifiManager.setTimeout(180);

    // Attempt to connect to WiFi
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
    reconnect(mac_address);

    for (int i = 0; i < sizeof(topics_to_sub)/sizeof(topics_to_sub[0]); i++) {
        client.subscribe(topics_to_sub[i]);
    }
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  setup_mqtt(String(WiFi.macAddress()));
  input_pins();
  output_pins();
}

void publish(const char* topic, int value) {
    delay(1000);  // Consider replacing this with a non-blocking delay
    char msg[200];
    snprintf(msg, 200, "{\n\"device_id\": \"%s\", \n\"reading\": \"%d\"\n}", client_id.c_str(), value);
    char print_msg[100];
    snprintf(print_msg, 100, "--- publishing message in topic %s ---", topic);
    Serial.println(print_msg);
    Serial.println(msg);
    client.publish(topic, msg, 1);
    delay(1000);  // Consider replacing this with a non-blocking delay
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
  publish(getTopicString(TOPIC_FLOATER), floater);
}

void loop() {
  if (!client.connected()) {
    reconnect(String(WiFi.macAddress()));
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