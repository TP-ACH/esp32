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
const int p_floater = 35;
const int p_reset = 14;

// output pins
const int p_lights = 4;
const int p_acid = 18;
const int p_base = 16;
const int p_water = 17;
const int p_nutes = 19;


// Other constants
const char* ap_password = "cleverleafy";
const char* mqtt_username = "arduino";
const char* mqtt_password = "arduino";


#endif // CONSTANTS_H