#include "leds.h"
#include <stdint.h>
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "WS2812.pio.h"

#define NUM_LEDS 12 
#define LED_PIN 14 // GPIO Pin 14 = DS1

static uint32_t led_data[NUM_LEDS]; // declare/initialise the led_data array

void leds_init() {
    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program); // refactored from start code
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
}

void led_set(int index, uint8_t r, uint8_t g, uint8_t b) { 
    // Reorder rgb to grb (as per WS2812 datasheet)
    led_data[index] = (g << 24) | (r << 16) | (b << 8); // green bits 31-24; red bits 23-16; blue bits 15-8 // store colour in array (index 0-11 represeting number of LEDs)
}

void led_commit() { 
    // "Push" the changes from led_set() to the LED
    for (int i=0; i < NUM_LEDS; i++) { // for each LED in the daisy-chain (0-11)
            pio_sm_put_blocking(pio0, 0, led_data[i]); // send its 24-bit BRG packet to the PIO state machine
        }
    // After all 12 packets are sent, PIO holds LOW for >208us (RESET) which latches the data and all LEDs update simultaneously (from datasheet)
}

void led_clear_all() {
    // Set every LED's stored colour value to 0 (i.e., off)
    for (int i = 0; i < NUM_LEDS; i++) { // for each LED in the daisy-chain (0-11)
        led_data[i] = 0; // turn off
    }
    led_commit(); // update the LED hardware to zero state
}