#include "tds_helper.h"

TDSHelper::TDSHelper(GravityTDS *tds) : tds(tds) {
}

void TDSHelper::setup_tds() {
    (*tds).setPin(p_ec);
    (*tds).setAref(5.0);  // reference voltage on ADC, default 5.0V on Arduino UNO
    (*tds).setAdcRange(1024);  // 1024 for 10bit ADC; 4096 for 12bit ADC
    (*tds).begin();
}


float TDSHelper::analog_read(float temperature) {
    (*tds).setTemperature(temperature); 
    (*tds).update();
    float ec = (*tds).getTdsValue();
    Serial.print("EC: ppm ");
    Serial.println(ec);
    return ec;
}