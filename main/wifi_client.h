#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <WiFi.h>
#include <WiFiManager.h>
#include "constants.h"


class MyWiFiClient {
public:
    // Constructor
    MyWiFiClient(WiFiClient client);

    // Public methods


private:
    WiFiClient wifi;
    void setup_wifi(WiFiClient wifi);
};

#endif 