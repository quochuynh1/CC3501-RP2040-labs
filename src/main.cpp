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

        // Assessment Demo 

        // 1. The user code can request to change a single LED, without having to remember all the others. this means
        //    your driver must memorise the previous values on the other LEDs so that they can be re-written during the
        //    update. 
        led_set(6, 255, 0, 0); // set the 7th LED to green
        led_commit(); // update only the 6th LED (the others remain off)
        led_clear_all(); // turn LED off
        sleep_ms(1500); // wait a little before moving to next demo

        // 2. The user code can make multiple changes to the LED information before "committing" the changes to the device.
        led_set(2, 0, 0, 255); // set 3rd LED to blue
        led_set(3, 0, 255, 0); // set 4th LED to red
        led_commit(); // update the two LEDs in hardware
        led_clear_all();
        sleep_ms(1500); // wait a little before moving to next demo

        // 3. As a convenience, there is a way to turn all the LEDs off. 
        led_clear_all();

    }

    return 0;
}
