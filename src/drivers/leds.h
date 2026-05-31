#pragma once // only include header once per build/compilation
#include <stdint.h> // allows uint8_t
struct LEDStatus { // define a struct for LED status
            uint8_t r, g, b; // three 8-bit unsigned intergers for red, green, and blue (each 0-255)
        };

class LEDS {
    public: // accessible by user code
        // static const int NUM_LEDS = 12; // define total number of LEDS in daisy chain
        int NUM_LEDS; // change from a static constant to a member variable which is set at runtime by init() to allow the user to customise the number of LEDs

        // Core Functions
        void init(int num_leds = 12); // initialise PIO0 to control the LED chain; takes number of LEDs in chain (default of 12)
        void set(int index, uint8_t r, uint8_t g, uint8_t b); // allow the user to change a single LED's colour 
        void commit(); // "Push" the changesto the physical LED/s
        void clear_all(); // turn all LEDs off    

        // Extra/Extension Functions
        void set_all(uint8_t r, uint8_t g, uint8_t b); // allow the user to change all LED colours at once
        void set_multiple(int* indices, int count, uint8_t r, uint8_t g, uint8_t b); // User provides an array of specific LEDs (e.g., int multiple_leds[] = {6, 7, 8}) to change colours; int* indices = array name; int count = length of array
        LEDStatus get(int index); // takes an LED index and returns its current RGB values as an LEDStatus struct
        void get_all(); // print the current RGB status of all LEDs to the serial monitor
        bool is_dirty();     // Returns true if set() has been called since the last commit(),indicating the buffer has changes that have not yet been written to the LEDs. Returns false if the buffer matches the current state of the LEDs.

        ~LEDS(); // destructor (manually frees memory now that we've moved from stack to heap allocation)

    private: // not accessible by user code
        static const int LED_PIN = 14; // define the GPIO pin that connects to first LED in daisy chain
        uint32_t* led_data = nullptr; // change led_data from a fixed array to a pointer (nullptr is initial value)
        bool dirty = false; // start as written to the LEDs
}; 