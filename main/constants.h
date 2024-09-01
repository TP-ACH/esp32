#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>
#define USE_SERIAL Serial

const char* topics_to_sub[] = {
  "actuators/light",
  "actuators/pumps/ph_down",
  "actuators/pumps/ph_up",
  "actuators/pumps/water",
  "actuators/pumps/nutrient"
};


// input pins
const int p_temperature = 12;
// p_sda is one of the pins used by the AHT10 sensor to gather temperature and humidity data
const int p_scl = 22;
const int p_pH = 27;
const int p_ec = 35;
// idem for p_sda for p_scl
const int p_sda = 21;
const int p_floater = 23;
const int p_reset = 0;

// output pins
const int p_lights = 2;
const int p_acid = 19;
const int p_base = 26;
const int p_water = 32;
const int p_nutes = 18;


// Other constants
const char* ap_password = "password123";
const char* mqtt_username = "test";
const char* mqtt_password = "test";

const int pin22 = 22;    // the number of the LED pin

#endif // CONSTANTS_H