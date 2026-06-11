#pragma once // only include header once per build/compilation
#include "pico/stdlib.h"

class MICROPHONE { 
    public: 
        void init();
        void read(uint16_t *adc_data, uint16_t number_of_sames);    
}; 