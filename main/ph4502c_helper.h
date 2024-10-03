#ifndef PH4502C_HELPER_H
#define PH4502C_HELPER_H

/// Default calibration value for the PH4502C sensor B parameter using linear regression.
#define PH4502C_DEFAULT_CALIBRATION 10.24352f

/// Slope for the calibration of the PH4502C sensor using linear regression.
#define PH4502C_SLOPE -1.68912f

/// Default reading interval (in milliseconds) between pH readings.
#define PH4502C_DEFAULT_READING_INTERVAL 100

/// Default number of pH readings to average.
#define PH4502C_DEFAULT_READING_COUNT 10

/// Default ADC resolution for the PH4502C sensor 12 bits for ESP32.
#define PH4502C_DEFAULT_ADC_RESOLUTION 4096.0f

/// Operating voltage for the PH4502C sensor using voltage devider to bring voltage down from 5 to 3.3.
#define PH4502C_VOLTAGE 3.3f


class pH4502cHelper {
public:

    pH4502cHelper(float calibration = PH4502C_DEFAULT_CALIBRATION,
                   int reading_interval = PH4502C_DEFAULT_READING_INTERVAL,
                   int reading_count = PH4502C_DEFAULT_READING_COUNT,
                   float adc_resolution = PH4502C_DEFAULT_ADC_RESOLUTION,
                   float slope = PH4502C_SLOPE)
        : _calibration(calibration), _reading_interval(reading_interval),
          _reading_count(reading_count), _adc_resolution(adc_resolution), 
          _slope(slope) {}

    void init(int p_ph);

    void recalibrate(float calibration);

    float read_ph_level();

    float get_last_voltage_reading() { return this->last_voltage; }

private:
    int _ph_level_pin;           ///< The analog pin connected to the pH level sensor.
    int _reading_interval;           ///< The interval between pH readings in milliseconds.
    int _reading_count;              ///< The number of readings to average.
    float _calibration;             ///< The pH calibration value.
    float _adc_resolution;          ///< The ADC resolution for analog-to-digital conversion.
    float _slope;                   ///< The slope for the calibration of the pH sensor.
    float last_reading;             ///< The last pH reading.
    float last_voltage;             ///< The last voltage reading.
};

#endif
