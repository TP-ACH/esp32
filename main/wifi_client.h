#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <WiFi.h>
#include <WiFiManager.h>
#include "constants.h"

class MyWiFiClient {
public:
    MyWiFiClient(WiFiClient *espClient);
    void setup_wifi();

private:
    WiFiClient *wifi;
};

#endif // WIFI_CLIENT_H