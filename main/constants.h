#ifndef CONSTANTS_H
#define CONSTANTS_H

char* topics[] = {
  "sensors/lights",
  "sensors/acid",
  "sensors/base",
  "sensors/water",
  "sensors/nutes"
};


// input pins
const int p_temperature = 12;
const int p_room_temperature = 14;
const int p_pH = 27;
const int p_ec = 26;
const int p_humidity = 25;
const int p_floater = 23;

// output pins
const int p_lights = 21;
const int p_acid = 19;
const int p_base = 18;
const int p_water = 4;
const int p_nutes = 2;


// Other constants
char* mqtt_broker = "192.168.100.11";
char* mqtt_username = "test";
char* mqtt_password = "test";
int mqtt_port = 1883;

const int pin22 = 22;    // the number of the LED pin

#endif