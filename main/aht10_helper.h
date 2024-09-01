#ifndef AHT10_HELPER_H
#define AHT10_HELPER_H

#include <AHTxx.h>

class AHT10Helper {
    public:
        AHT10Helper(AHTxx *aht10);
        void setup_aht10(int p_sda, int p_scl);
        float read_temperature();
        float read_humidity();
    private:
       AHTxx *aht10;
};

#endif