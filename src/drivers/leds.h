#pragma once // only include header once per build/compilation
#include <stdint.h> // allows uint8_t

class LEDS {
    public: // accessible by user code
        static const int NUM_LEDS = 12; // define total number of LEDS in daisy chain
        void init(); // initialise PIO0 to control the LED chain
        void set(int index, uint8_t r, uint8_t g, uint8_t b); // allow the user to change a single LED's colour 
        void set_all(uint8_t r, uint8_t g, uint8_t b); // all the user to change all LED colours at once
        void commit(); // "Push" the changesto the physical LED/s
        void clear_all(); // turn all LEDs off    
    private: // not accessible by user code
        static const int LED_PIN = 14; // define the GPIO pin that connects to first LED in daisy chain
        uint32_t led_data[NUM_LEDS] = {}; // zero initialised array which stores the colour of the LED (index = 0-11)
}; 