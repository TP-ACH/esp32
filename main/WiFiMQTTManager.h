/*
  WiFiMQTTManager.h - Library for the ESP8266/ESP32 Arduino platform
  for configuration of WiFi and MQTT credentials using a AP mode and Captive Portal
  Written by David Reed hashmark47@gmail.com
  GNU license  
*/
#ifndef WiFiMQTTManager_h
#define WiFiMQTTManager_h

//#define MQTT_MAX_PACKET_SIZE 512

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <PubSubClient.h>
#include "Arduino.h"
#include "FS.h"
#include "constants.h"

#ifdef ESP32
  #include <SPIFFS.h>
#endif

void _settingsAP();

class WiFiMQTTManager {
  public:
    WiFiMQTTManager(int resetPin, const char* APpassword);
    void setup();
    void loop();
    void setDebugOutput(bool b);
    std::shared_ptr<PubSubClient> client;
    char clientId[40];
    char deviceId[40];
    char chipId[40];
    void (*subscribeTo)();
    void (*subscriptionCallback)(char* topicIn, byte* message, unsigned int length);
    bool formatFS;
    long lastMsg;
    char deviceType[40];
  private:
    void _setupSpiffs();
    void _checkButton();
    void _registerDevice();
    void _reconnect();
    void _subscribeToServices();
    int _resetPin;
    char _friendly_name[40];
    // add mqtt_username and mqtt_password for MQTT authentication
    char _mqtt_server[40];
    char _mqtt_port[6];
    char* _APpassword;
    int _LED_BUILTIN;
    long _lastMsg;
    char _msg[50];
    int _value;
    bool _shouldSaveConfig;  
    WiFiClient _espClient;

};

#endif