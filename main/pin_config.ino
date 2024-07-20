#include "cleverleafy.h"

void input_pins() {
  pinMode(p_temperature, INPUT);
  pinMode(p_room_temperature, INPUT);
  pinMode(p_pH, INPUT);
  pinMode(p_ec, INPUT);
  pinMode(p_humidity, INPUT);
  pinMode(p_floater, INPUT);
}

void output_pins() {
  pinMode(p_acid, OUTPUT);
  pinMode(p_base, OUTPUT);
  pinMode(p_lights, OUTPUT);
  pinMode(p_water, OUTPUT);
  pinMode(p_nutes, OUTPUT);
}