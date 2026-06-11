#pragma once // only include header once per build/compilation
#include "pico/stdlib.h"

class MICROPHONE { 
    public: 
        void init(); // a function to initialise the microphone and ADC
        void read(uint16_t *adc_data, uint16_t number_of_samples); // read number_of_samples from the microphone into adc_data
}; 