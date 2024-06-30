#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

#define USE_SERIAL Serial

const char* ssid = "PENIA323";
const char* password = "nope";

const char* mqtt_broker = "192.168.100.11";
const char* topic_be = "sensors/temperature";
const char* mqtt_username = "test";
const char* mqtt_password = "test";
const int mqtt_port = 1883;

const int pin22 = 22;    // the number of the LED pin

WiFiClient espClient;
PubSubClient client(espClient);

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
}

void reconnect() {
  while (!client.connected()) {
    digitalWrite(pin22, HIGH);
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());

    Serial.printf("The client %s connects to the MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("MQTT broker connected");
      digitalWrite(pin22, LOW);
      client.subscribe(topic_be);
    } else {
      Serial.print("Failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(pin22, OUTPUT);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(pin22, HIGH);
    delay(500);
    Serial.print(".");
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
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}