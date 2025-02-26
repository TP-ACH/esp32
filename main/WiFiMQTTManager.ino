/*
WiFiMQTTManager.cpp - Library for the ESP8266/ESP32 Arduino platform
for configuration of WiFi and MQTT credentials using a Captive Portal
Written by David Reed hashmark47@gmail.com
GNU license
*/
#include "WiFiMQTTManager.h"

WiFiManager * wm;

WiFiMQTTManager::WiFiMQTTManager(int resetPin, const char * APpassword) {
  wm = new WiFiManager;

  lastMsg = 0;
  formatFS = false;
  _APpassword = (char *)APpassword;
  strcpy(_mqtt_server, "192.168.100.YOURIP");
  strcpy(_mqtt_port, "1883");
  _LED_BUILTIN = 2;
  _lastMsg = 0;
  _value = 0;
  _shouldSaveConfig = false;

  Serial.begin(115200);
  void _placeholderSubscibeTo();
  subscribeTo = _placeholderSubscibeTo;
  void _subscriptionCallback(char * topicIn, byte * message, unsigned int length);
  subscriptionCallback = _subscriptionCallback;
  wm -> setDebugOutput(false);
  pinMode(resetPin, INPUT);
  wm -> setTimeout(180);
  _resetPin = resetPin;
  #ifdef ESP32
  strcpy(deviceType, "ESP32");
  #elif defined(ESP8266)
  strcpy(deviceType, "ESP8266");
  #else
  strcpy(deviceType, "UNKNOWN");
  #endif

}

void WiFiMQTTManager::loop() {
  _checkButton();
  if (!client -> connected()) {
    _reconnect();
  }
  client -> loop();
}

void WiFiMQTTManager::setup() {
  WiFi.begin();

  String _chipId = String(WIFI_getChipId(), HEX);
  String _mac = String(WiFi.macAddress());
  _mac.toLowerCase();
  _mac.replace(":", "");
  _mac.replace("240ac4", "a"); // vendor = Espressif Inc.
  String _clientId = "ESP" + _mac;

  strcpy(chipId, _chipId.c_str());
  strcpy(clientId, _clientId.c_str());
  strcpy(deviceId, _clientId.c_str());

  Serial.print("WMM: clientId: ");
  Serial.println(clientId);

  _setupSpiffs();
  wm -> setSaveConfigCallback([&]{
    Serial.println("WMM: should save config...");
    _shouldSaveConfig = true;
  });
  wm -> setClass("invert"); // dark theme

  WiFiManagerParameter custom_friendly_name("name", "Friendly Name", _friendly_name, 40);
  WiFiManagerParameter custom_mqtt_server("server", "MQTT Server", _mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT Port", _mqtt_port, 6);
  // add mqtt_username and mqtt_password for MQTT authentication
  wm -> addParameter( & custom_friendly_name);
  wm -> addParameter( & custom_mqtt_server);
  wm -> addParameter( & custom_mqtt_port);
  // add mqtt_username and mqtt_password for MQTT authentication

  wm -> setAPCallback([ & ](WiFiManager * myWiFiManager) {
    Serial.println("WMM: entering config mode...");
    Serial.print("WMM: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("WMM: connect your device to WiFi SSID ");
    Serial.print(myWiFiManager -> getConfigPortalSSID());
    Serial.println(" to configure WiFi and MQTT...");
  });

  Serial.println("WMM: lets try to connect to WiFi...");
  if (!wm -> autoConnect(clientId, _APpassword)) {
    Serial.println("WMM: failed to connect and hit timeout...");
    delay(3000);
    ESP.restart();
    delay(5000);
  }
  Serial.println("WMM: connected to WiFi!!...");

  //read updated parameters
  strcpy(_friendly_name, custom_friendly_name.getValue());
  strcpy(_mqtt_server, custom_mqtt_server.getValue());
  strcpy(_mqtt_port, custom_mqtt_port.getValue());
  // add mqtt_username and mqtt_password for MQTT authentication

  //save the custom parameters to FS
  if (_shouldSaveConfig) {
    Serial.println("WMM: saving config...");
    JsonDocument doc;
    doc["friendly_name"] = _friendly_name;
    doc["mqtt_server"] = _mqtt_server;
    doc["mqtt_port"] = _mqtt_port;
    // add mqtt_username and mqtt_password for MQTT authentication
    serializeJson(doc, Serial);

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("WMM: failed to open config file for writing...");
    }

    serializeJson(doc, Serial);
    serializeJson(doc, configFile);
    configFile.close();
    _shouldSaveConfig = false;
  }

  Serial.println("WMM: ");
  Serial.print("WMM: local IP:");
  Serial.println(WiFi.localIP());

  unsigned short port = (unsigned short) strtoul(_mqtt_port, NULL, 0);

  Serial.print("WMM: _mqtt_server: ");
  Serial.println(_mqtt_server);
  Serial.print("WMM: _mqtt_port: ");
  Serial.println(_mqtt_port);

  Serial.print("WMM: clientId: ");
  Serial.println(clientId);

  client.reset(new PubSubClient(_espClient));
  client -> setServer(_mqtt_server, port);

  Serial.print("WMM: attempting MQTT connection...");
  if (!client -> connect(clientId, mqtt_username, mqtt_password)) {
    Serial.println("failed to connect to MQTT...");
    delay(3000);
    wm -> resetSettings();
    ESP.restart();
    delay(5000);
  } else {
    Serial.println("mqtt connected...via setup...");
    subscribeTo();
    _subscribeToServices();
    client -> setCallback(subscriptionCallback);
  }
  _registerDevice();
}

void WiFiMQTTManager::_setupSpiffs() {
  if (formatFS == true) {
    Serial.print("WMM: formatting FS...please wait..... ");
    SPIFFS.format();
  }

  //read configuration from FS json
  Serial.println("WMM: mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("WMM: mounted file system...");
    if (SPIFFS.exists("/config.json")) {
      Serial.println("WMM: reading config file...");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("WMM: opened config file...");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr < char[] > buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, buf.get());
        serializeJson(doc, Serial);
        if (!error) {
          Serial.println("\nWMM: parsed json...");
          strcpy(_friendly_name, doc["friendly_name"]);
          strcpy(_mqtt_server, doc["mqtt_server"]);
          strcpy(_mqtt_port, doc["mqtt_port"]);
         // add mqtt_username and mqtt_password for MQTT authentication
        } else {
          Serial.println("WMM: failed to load json config...");
        }
      }
    } else {
      Serial.println("WMM: could not find config file...");
    }
  } else {
    Serial.println("WMM: failed to mount FS");
    Serial.println("WMM: formating FS...re-upload to try again...");
    SPIFFS.format();
  }
}

void WiFiMQTTManager::_checkButton() {
  if (digitalRead(_resetPin) == HIGH) {
    delay(50);
    if (digitalRead(_resetPin) == HIGH) {
      Serial.println("WMM: button Pressed...");
      delay(3000); 
      if (digitalRead(_resetPin) == HIGH) {
        Serial.println("WMM: button held...");
        Serial.println("WMM: erasing config, restarting...");
        wm -> resetSettings();
        ESP.restart();
      }

      _settingsAP();
      return;

    }
  }
}

void WiFiMQTTManager::_reconnect() {
  while (!client -> connected()) {
    Serial.print("WMM: attempting MQTT connection...");
    if (client -> connect(clientId) == true) {
      Serial.println("mqtt connected...via reconnect loop...");
      subscribeTo();
      _subscribeToServices();
      client -> setCallback(subscriptionCallback);
    } else {
      Serial.print("mqtt connect failed, rc=");
      Serial.print(client -> state());
      Serial.println(" try again in 5 seconds...");
      _checkButton();
      delay(5000);
    }
  }
}
void WiFiMQTTManager::setDebugOutput(bool b) {
  wm -> setDebugOutput(b);
}

void WiFiMQTTManager::_registerDevice() {

  JsonDocument doc;


  doc["deviceType"] = deviceType;
  doc["deviceId"] = deviceId;
  doc["name"] = _friendly_name;
  doc["chipId"] = chipId;

  char topic[200];
  char messageBuffer[2000];
  snprintf(topic, sizeof(topic), "%s%s", "deviceLog/", deviceId);

  serializeJson(doc, Serial);

  Serial.print("Sending message to MQTT topic: ");
  Serial.println(topic);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.print("messageBuffer: ");
  Serial.println(messageBuffer);

  if (client -> publish(topic, messageBuffer) == true) {
    Serial.println("WMM: Success sending message to register the device...");
  } else {
    Serial.println("WMM: Error sending message to register the device...");
  }

}

void _placeholderSubscibeTo() {
  Serial.println("WMM: ....placeholderSubscibeTo called...");
}

void WiFiMQTTManager::_subscribeToServices() {
  char topic[100];
  snprintf(topic, sizeof(topic), "%s%s", "service/", deviceId);
  client -> subscribe(topic);
}

void _settingsAP() {
  Serial.println("WMM: starting config portal...");

  wm -> resetSettings();
  ESP.restart();

}

void _subscriptionCallback(char * topicIn, byte * message, unsigned int length) {
  Serial.print("WMM: Message arrived on topic: ");
  Serial.print(topicIn);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char) message[i]);
    messageTemp += (char) message[i];
  }
  Serial.println();

  if (messageTemp == "restart") {
    Serial.println("RESTARTING NOW!!!!");
    ESP.restart();
  }

  if (messageTemp == "format FS") {
    Serial.println("FORMATTING FS NOW!!!!");
    Serial.print("WMM: formatting FS...please wait..... ");
    SPIFFS.format();
    ESP.restart();
  }

  if (messageTemp == "settingsAP") {
    Serial.println("STARTING Settings AP NOW!!!!");
    _settingsAP();
  }
}