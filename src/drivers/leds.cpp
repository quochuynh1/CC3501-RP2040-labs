#include "leds.h"
#include <stdint.h>
#include <stdio.h> // allows printf
#include <stdlib.h> // allows calloc
#include "pico/stdlib.h" // allows sleep_us(300)
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "WS2812.pio.h"


void LEDS::init(int num_leds, int LED_PIN) { // takes number of LEDs in chain (default of 12)
    // Initialise PIO0 to control the LED chain
    LEDS::NUM_LEDS = num_leds; // store the number of leds as a class member variable so the rest of the driver knows how many LEDs there are (i.e., remember how many LEDs the user wants)
    led_data = (uint32_t*)calloc(NUM_LEDS, sizeof(uint32_t)); // heap-allocate an array of NUM_LEDS uint32_t values and point led_data at that memory (zeroed due to calloc) (i.e., reserve memory for the LED colour array at runtime)

    uint pio_program_offset = pio_add_program(pio0, &ws2812_program); // refactored from start code
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);

    printf("NUM_LEDS = %d\n", NUM_LEDS); // debug print NUM_LEDS in serial monitor to test it was set correctly
}

LEDS::~LEDS() {
    free(led_data); // release heap memory when LEDS object is destroyed (avoids memory leak)
}

void LEDS::set(int index, uint8_t r, uint8_t g, uint8_t b) { 
    // Note: don't need to reorder as per WS2812 because using dome top LED rather than flat top
    led_data[index] = (r << 24) | (g << 16) | (b << 8); // red bits 31-24; green bits 23-16; blue bits 15-8 // store colour in array (index 0-11 LED number)
    LEDS::dirty = true; // update the dirty bool flag to true
}

void LEDS::set_all(uint8_t r, uint8_t g, uint8_t b) { 
    // Allow the user to set all LEDs to a single colour at once
    for(int i = 0; i < LEDS::NUM_LEDS; i++) { // for all 12 LEDs (index 0-11)
        LEDS::set(i, r, g, b); // set their colour to 
    } 
}

void LEDS::commit() { 
    // "Push" the changes to the physical LED/s
    for (int i=0; i < NUM_LEDS; i++) { // for each LED in the daisy-chain (0-11)
            pio_sm_put_blocking(pio0, 0, led_data[i]); // send its 24-bit BRG packet to the PIO state machine
        } 
        sleep_us(300); // after all 12 packets are sent, PIO holds LOW for >280us (RESET) which latches the data and all LEDs update simultaneously (from datasheet)
    LEDS::dirty = false; // update the dirty bool flag to false
}

void LEDS::clear_all() {
    // Set every LED's stored colour value to 0 (i.e., off)
    LEDS::set_all(0, 0, 0);
    LEDS::commit(); // push changes to the board
}

void LEDS::set_multiple(int* indices, int count, uint8_t r, uint8_t g, uint8_t b) {
    // User provides an array of specific LEDs (e.g., int multiple_leds[] = {6, 7, 8}) to change colours
    // int* indices = array name
    // int count = length of array
    for (int i = 0; i < count; i++) { // iterate over each index in the array
        set(indices[i], r, g, b); // set the LED at that index to the given colour
    }
}

LEDStatus LEDS::get(int index, int LED_PIN) {
    // Takes an LED index and returns its current RGB values as an LEDStatus struct
    LEDStatus colour; // create an empty LEDStatus variable to store the results
    colour.r = led_data[index] >> 24 & 0xFF; // extract red by shifting bits 31-24 down to 7-0, mask to isolate bottom 8 bits
    colour.g = led_data[index] >> 16 & 0xFF; // extract green by shifting bits 23-16 down to 7-0, mask to isolate bottom 8 bits
    colour.b = led_data[index] >> 8 & 0xFF; // extract blue by shifting bits 15-8 down to 7-0, mask to isolate bottom 8 bits
    return colour;  // return the LEDStatus struct variable
}

void LEDS::get_all(int LED_PIN) { 
    // Queries and prints the current RGB values of all LEDs to the serial monitor.
    // LED numbering is displayed as 1-12 (maps internally to index 0-11).
    for (int i = 0; i < LEDS::NUM_LEDS; i++) { // for all 12 LEDs (index 0-11)
        LEDStatus status = get(i, LED_PIN); // iteratively query all 12 LEDs
        printf("LED %d: R=%d G=%d B=%d\n", i + 1, status.r, status.g, status.b); // print in serial port
    }
}

bool LEDS::is_dirty() { 
    // Indicates whether current values have been updated but not yet written to the LEDs (true (1) means they have not been committed yet)
    return dirty; // returns true (1) or false (0)
}

void LEDS::set_hsv(int index, float h, float s, float v) { 
    // Conver from user given hsv to RGB so that leds.set() can communicate with the LEDs
    // Help from: https://stackoverflow.com/questions/51203917/math-behind-hsv-to-rgb-conversion-of-colors
    float r, g, b; // declare the three float variables for red, green, and blue respectively (e.g., R = 0.75; 75% red)
    
    int i = (int)(h / 60); //  calculate which sector of the hue spectrum currently in (0-5). hue is 0-360, there are 6 sectors each with 60 degrees
    float f = (h / 60.0f) - i; // fractional position within the current sector (0.0-1.0)
    float p = v * (1.0f - s); // minimum brightness (decreases as saturation increases)
    float q = v * (1.0f - f * s); // falling edge value (channel fading out across this secotr)
    float t = v * (1.0f - (1.0f - f) * s); // rising edge value (channell fading in across this sector)

    switch(i % 6) { // assign RGB based on which sector the given hue falls within
        case 0: r = v; g = t; b = p; break; // red to yellow
        case 1: r = q; g = v; b = p; break; // yellow to grreen
        case 2: r = p; g = v; b = t; break; // green to cyan
        case 3: r = p; g = q; b = v; break; // cyan to blue
        case 4: r = t; g = p; b = v; break; // blue to magenta
        case 5: r = v; g = p; b = q; break; // magenta back to red
        default: r = g = b = 0; break; // safety case
    }
    set(index, (uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255)); // convert floats (0.0-1.0) to uint8_t (0-255) and pass to set()
}