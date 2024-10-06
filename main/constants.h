#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>
#define USE_SERIAL Serial

const char* topics_to_sub[] = {
  "actuators/light_on",
  "actuators/light_off",
  "actuators/pumps/ph_down",
  "actuators/pumps/ph_up",
  "actuators/pumps/water",
  "actuators/pumps/nutrient",
  "sensors/enable",
  "sensors/disable"
};


// input pins
// p_scl is one of the pins used by the AHT10 sensor to gather temperature and humidity data
const int p_scl = 22;
const int p_pH = 33;
const int p_ec = 34;
// idem for p_sda for p_scl
const int p_sda = 21;
const int p_floater = 34;
const int p_reset = 14;

// output pins
const int p_lights = 4;
const int p_acid = 19;
const int p_base = 26;
const int p_water = 23;
const int p_nutes = 18;


// Other constants
const char* ap_password = "password123";
const char* mqtt_username = "test";
const char* mqtt_password = "test";


#endif // CONSTANTS_H