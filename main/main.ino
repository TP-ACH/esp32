#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Personal-WiFi-F81-2.4Ghz";
const char* password = "JfBsuE2EXp";

const char* mqtt_broker = "192.168.1.14";
const char* topic_be = "testtopic";
const char* mqtt_username = "test";
const char* mqtt_password = "test";
const int mqtt_port = 9001;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting to WiFi...");
  }

  Serial.println("");
  Serial.println("Conectado a WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(server, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client";
    Serial.printf("The client %s connects to the MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str())){//, mqtt_username, mqtt_password)) {
      Serial.println("MQTT broker connected");
    } else {
      Serial.println("failed with state");
      Serial.println(client.state());
      delay(2000);
    }
  }

}

void callback(char *topic, byte *payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("--------");
}

void loop() {
  client.loop();
}