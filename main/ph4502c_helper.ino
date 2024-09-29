#include "ph4502c_helper.h"

void pH4502cHelper::init(int p_ph) {
    this->_ph_level_pin = p_ph;
    pinMode(this->_ph_level_pin, INPUT);
}

void pH4502cHelper::recalibrate(float calibration) {
    this->_calibration = calibration;
}

float pH4502cHelper::read_ph_level() {
    float reading = 0.0f;

    for(int i = 0; i < this->_reading_count; i++) {
        reading += analogRead(this->_ph_level_pin);
        delayMicroseconds(this->_reading_interval);
    }

    reading /= this->_reading_count;
    reading = (PH4502C_VOLTAGE / this->_adc_resolution) * reading;
    this-> last_voltage = reading;
    reading = this->_calibration + reading * this->_slope;
    this->last_reading = reading;
    return reading;
}
