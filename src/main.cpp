#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#define LED_PIN 14
#define NUM_LEDS 12 

int main()
{
    stdio_init_all();

    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
    uint32_t led_data [1];

    for (;;) {
        // Test the log system
        log(LogLevel::INFORMATION, "Hello world");

        // Turn on the first LED to be a certain colour
        // uint8_t red = 0;
        // uint8_t green = 0;
        // uint8_t blue = 255;
        // led_data[0] = (green << 24) | (red << 16) | (blue << 8); // Update to align with WS2812 protocol from datasheet
        // pio_sm_put_blocking(pio0, 0, led_data[0]);
        // sleep_ms(500);

        uint32_t led_data[NUM_LEDS]; // declare/initialise the led_data array

        led_data[0] = (0 << 24) | (255 << 16) | (0 << 8); // red
        led_data[1] = (255 << 24) | (0 << 16) | (0 << 8); // green
        led_data[2] = (0 << 24) | (0 << 16) | (255 << 8); // blue
        
        for (int i=0; i < NUM_LEDS; i++) { // for each LED in the daisy-chain (0-11)
            pio_sm_put_blocking(pio0, 0, led_data[i]); // send its 24-bit colour packet to the PIO
        }
        sleep_ms(500); 

        for (int i=0; i < NUM_LEDS; i++) { // for each LED in the daisy-chain (0-11)
            led_data[i] = 0; // set the led colour off
            pio_sm_put_blocking(pio0, 0, led_data[i]); // send it to the PIO
        }
        sleep_ms(500);

        // Set the first LED off 
        // led_data[0] = 0;
        // pio_sm_put_blocking(pio0, 0, led_data[0]);
        // sleep_ms(500);
    }

    return 0;
}
