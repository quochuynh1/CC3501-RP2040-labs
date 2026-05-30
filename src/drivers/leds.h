#pragma once // only include header once per build/compilation
#include <stdint.h> // allows uint8_t
struct LEDStatus { // define a struct for LED status
            uint8_t r, g, b; // three 8-bit unsigned intergers for red, green, and blue (each 0-255)
        };

class LEDS {
    public: // accessible by user code
        static const int NUM_LEDS = 12; // define total number of LEDS in daisy chain
        // Core Functions
        void init(); // initialise PIO0 to control the LED chain
        void set(int index, uint8_t r, uint8_t g, uint8_t b); // allow the user to change a single LED's colour 
        void commit(); // "Push" the changesto the physical LED/s
        void clear_all(); // turn all LEDs off    

        // Extra/Extension Functions
        void set_all(uint8_t r, uint8_t g, uint8_t b); // allow the user to change all LED colours at once
        void set_multiple(int* indices, int count, uint8_t r, uint8_t g, uint8_t b); // User provides an array of specific LEDs (e.g., int multiple_leds[] = {6, 7, 8}) to change colours; int* indices = array name; int count = length of array
        LEDStatus get(int index); // takes an LED index and returns its current RGB values as an LEDStatus struct
        void get_all(); // print the current RGB status of all LEDs to the serial monitor

    private: // not accessible by user code
        static const int LED_PIN = 14; // define the GPIO pin that connects to first LED in daisy chain
        uint32_t led_data[NUM_LEDS] = {}; // zero initialised array which stores the colour of the LED (index = 0-11)
}; 