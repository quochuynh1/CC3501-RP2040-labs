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
        sleep_ms(2000);

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

        // Test Extension Functions 
        // 2. Add a way to update multiple LEDs in a single funciton call. 
        int multiple_leds[] = {8, 9, 10, 11}; // right panel LEDs
        leds.set_multiple(multiple_leds, 4, 0, 0, 255); // turn them blue
        sleep_ms(1000);
        leds.clear_all(); 
        sleep_ms(1000);

        // 3. Prorvide a way to query the current status of the LEDs
        int led_index = 3; //
        leds.set(led_index, 0, 255, 255); // set LED 4 to aqua
        leds.commit();
        LEDStatus status = leds.get(3); // query LED 3 only
        printf("LED %d: R=%d G=%d B=%d\n", led_index, status.r, status.g, status.b); // print in serial port
        
        sleep_ms(1000);
        leds.clear_all(); 
        sleep_ms(1000);

        leds.set_all(60, 70, 80); // set all LEDs to 
        leds.commit(); 
        leds.get_all(); // query status of all LEDs

        sleep_ms(1000);
        leds.clear_all(); 
        sleep_ms(1000);

        // 4. Allow the user code to query whether current values have been
        // updated but not yet written to the LEDs.
        leds.set(0, 255, 0, 0);
        printf("Dirty: %d\n", leds.is_dirty()); // should print 1 (true) as changes have not been written

        leds.commit();
        printf("Dirty: %d\n", leds.is_dirty()); // should print 0 (false) as changes have been written

        sleep_ms(1000);
        leds.clear_all(); 
        sleep_ms(1000);
}
    return 0;
}