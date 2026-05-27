#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#include "drivers/leds.h"

#define LED_PIN 14
#define NUM_LEDS 12 

int main()
{
    stdio_init_all();
    leds_init();

    for (;;) {
        // Test the log system
        log(LogLevel::INFORMATION, "Hello world");
        
        // Testing LEDs
        for (int i = 0; i < NUM_LEDS; i++) { // for all 12 LEDs
            led_set(i, rand() % 256, rand() % 256, rand() % 256); // rand() generates large integer, % 256 which will always be between 0-255
        }
        led_commit(); // update LED colour in hardware
        sleep_ms(500); // wait a little

        // Turn all off
        led_clear_all(); // update LED colour in hardware
        sleep_ms(500); // wait a little
    }

    return 0;
}
