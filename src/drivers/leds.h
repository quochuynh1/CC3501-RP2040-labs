#pragma once
#include <stdint.h>

// Initialise the LED driver (call once in main)
void leds_init();

// Set a single LED colour without affecting others
void led_set(int index, uint8_t r, uint8_t g, uint8_t b);

// Send current state to the hardware
void led_commit();

// Turn all LEDs off
void led_clear_all();