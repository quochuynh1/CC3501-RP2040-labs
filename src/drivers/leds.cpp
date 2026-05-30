#include "leds.h"
#include <stdint.h>
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "WS2812.pio.h"


void LEDS::init() {
    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program); // refactored from start code
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
}

void LEDS::set(int index, uint8_t r, uint8_t g, uint8_t b) { 
    // Note: don't need to reorder as per WS2812 because using dome top LED rather than flat top
    led_data[index] = (r << 24) | (g << 16) | (b << 8); // red bits 31-24; green bits 23-16; blue bits 15-8 // store colour in array (index 0-11 LED number)
}

void LEDS::set_all(uint8_t r, uint8_t g, uint8_t b) { 
    // Allow the user to set all LEDs to a single colour at once
    for(int i = 0; i < LEDS::NUM_LEDS; i++) { // for all 12 LEDs (index 0-11)
        LEDS::set(i, r, g, b); // set their colour to 
    } 
    LEDS::commit();
}

void LEDS::commit() { 
    // "Push" the changes to the physical LED/s
    for (int i=0; i < NUM_LEDS; i++) { // for each LED in the daisy-chain (0-11)
            pio_sm_put_blocking(pio0, 0, led_data[i]); // send its 24-bit BRG packet to the PIO state machine
        } // After all 12 packets are sent, PIO holds LOW for >208us (RESET) which latches the data and all LEDs update simultaneously (from datasheet)
}

void LEDS::clear_all() {
    // Set every LED's stored colour value to 0 (i.e., off)
    LEDS::set_all(0, 0, 0);
}