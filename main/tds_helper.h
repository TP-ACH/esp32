#ifndef TDS_HELPER_H
#define TDS_HELPER_H

#include "constants.h"
#include "GravityTDS.h"

class TDSHelper {
public:
    TDSHelper(GravityTDS *tds);
    void setup_tds();
    float analog_read(float temperature);

private:
    GravityTDS *tds;
};


#endif // TDS_HELPER_H