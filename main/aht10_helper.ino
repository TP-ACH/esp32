#include "aht10_helper.h"

AHT10Helper::AHT10Helper(AHTxx *aht10) : aht10(aht10) {
}

void AHT10Helper::setup_aht10(int p_sda, int p_scl) {
    while ((*aht10).begin(p_sda, p_scl) != true) {
        Serial.println("Could not find AHT10 sensor!");
        Serial.print("aht10 status: ");
        Serial.println((*aht10).getStatus());
        delay(1000);
    }
}


float AHT10Helper::read_temperature() {
    return (*aht10).readTemperature();
}

float AHT10Helper::read_humidity() {
    //measurement with high frequency leads to heating of the sensor
    delay(2000);
    return (*aht10).readHumidity();
}