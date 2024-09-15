#include "ph4502c_helper.h"

void pH4502cHelper::init(int p_temp, int p_ph) {
    this->_ph_level_pin = p_ph;
    this->_temp_pin = p_temp;
    pinMode(this->_ph_level_pin, INPUT);
    pinMode(this->_temp_pin, INPUT);
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
    reading = this->_calibration + reading * this->_slope;

    return reading;
}

int pH4502cHelper::read_temp() {
    float reading = 0.0f;

    for(int i = 0; i < this->_reading_count; i++) {
        reading += analogRead(this->_temp_pin);
        delayMicroseconds(this->_reading_interval);
    }
     reading /= this->_reading_count;
    reading = (PH4502C_VOLTAGE / this->_adc_resolution) * reading;
    float temperature = 5 * reading + 9.549;
    return temperature;
}
