#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds.h"

int main()
{
    stdio_init_all();
    LEDS leds; // create instance of LEDS class
    leds.init(); // initialise the LED drier

    for (;;) {
        // Test the log system
        log(LogLevel::INFORMATION, "Hello world");  

        // LED Assessment Demo
        // 1. The user code can request to change a single LED, without having
        // to remember all the others. This means your driver must memorise the 
        // previous values on the other LEDs so that they can be
        leds.set(0, 255, 0, 0); // LED 1 red
        leds.set(1, 0, 255, 0); // LED 2 green
        leds.set(2, 0, 0, 255); // LED 3 blue
        leds.commit(); // turn first 3 LEDs RGB              
        sleep_ms(1000);

        leds.set(1, 255, 255, 255); // turn middle of those three white (shows changing of single LED
        leds.commit();              // while the others remain the same)
        sleep_ms(1000); 


        // 2. The user code can make multiple changes to the LED information
        // before “committing” the changes to the device. For instance:
        leds.set(3, 255, 0, 255);
        leds.set(4, 255, 0, 255);           
        leds.set(5, 255, 0, 255);
        leds.set(6, 255, 0, 255);
        leds.set(7, 255, 0, 255);
        leds.set(8, 255, 0, 255);
        leds.set(9, 255, 0, 255);
        leds.set(10, 255, 0, 255);
        leds.set(11, 255, 0, 255);
        leds.commit(); // turn the rest of the LEDs purple 
        sleep_ms(1000);

        leds.clear_all();
        sleep_ms(1000); 
        for (int i = 0; i < leds.NUM_LEDS; i++) {
        leds.set(i, 0, 255, 0); // set current LED green
        leds.commit();
        sleep_ms(200);
        }
        sleep_ms(1000);

        // 3. As a convenience, there is a way to turn all the LEDs off.
        leds.clear_all();
        sleep_ms(1000); 

        leds.set_all(255, 100, 0); // set all LEDs to orange at once
        sleep_ms(1000);

        leds.clear_all();
        sleep_ms(1000);
}


    return 0;
}
