#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds.h"
#include "drivers/lis3dh.h"
#include "board.h"
#include <math.h>

void run_long_leds_test(LEDS &leds) { 
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

    // Laurence - "write code so that the you can change the colour of ONE led, then change it back"
    for(int i = 0; i < 10; i++) { 
        leds.set(0, 255, 0, 0); 
        leds.commit(); 
        sleep_ms(200); 

        leds.set(0, 0, 0, 255); 
        leds.commit(); 
        sleep_ms(200); 
    }

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
    int multiple_leds1[] = {0, 1, 2, 3}; // left panel LEDs
    leds.set_multiple(multiple_leds1, 4, 255, 0, 0); // turn them red
    
    int multiple_leds2[] = {4, 5, 6, 7}; // middle panel LEDs
    leds.set_multiple(multiple_leds2, 4, 0, 0, 255); // turn them blue

    int multiple_leds3[] = {8, 9, 10, 11}; // right panel LEDs
    leds.set_multiple(multiple_leds3, 4, 0, 255, 0); // turn them green

    leds.commit(); 
    sleep_ms(1000);
    leds.clear_all(); 
    sleep_ms(1000);

    // 3. Prorvide a way to query the current status of the LEDs
    int led_index = 3; //
    leds.set(led_index, 0, 255, 255); // set LED 4 to aqua
    leds.commit();
    LEDStatus status = leds.get(3, LED_PIN); // query LED 3 only
    printf("LED %d: R=%d G=%d B=%d\n", led_index, status.r, status.g, status.b); // print in serial port
    
    sleep_ms(1000);
    leds.clear_all(); 
    sleep_ms(1000);

    leds.set_all(0, 255, 255); // set all LEDs to cyan 
    leds.commit(); 
    leds.get_all(LED_PIN); // query status of all LEDs

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

    // 6. Implement alternative colour models such as HSV to make it
    // easier to produce visually interesting animations.
    for (int i = 0; i < leds.NUM_LEDS; i++) { 
    leds.set_hsv(i, (360.0f / leds.NUM_LEDS) * i, 1.0f, 1.0f); // divides 360 degrees across 12 LEDs (i.e., LED 0 gets red, LED 6 gets cyan, etc.)
    }
    leds.commit();
    sleep_ms(2000);
    leds.clear_all(); 
    sleep_ms(1000);
}

void run_short_leds_test(LEDS &leds) { 
    // 1. Single LED change - others remain unchanged
    leds.set(0, 255, 0, 0); // LED 0 red
    leds.set(1, 0, 255, 0); // LED 1 green
    leds.set(2, 0, 0, 255); // LED 2 blue
    leds.commit();
    sleep_ms(1000);

    leds.set(1, 255, 255, 255); // change only LED 1 to white; 0 and 2 unchanged
    leds.commit();
    sleep_ms(1000);

    leds.set(1, 0, 255, 0); // change back to green
    leds.commit();
    sleep_ms(1000);

    // 2. Batch multiple changes before a single commit
    leds.set(3, 255, 0, 255);
    leds.set(4, 255, 0, 255);
    leds.set(5, 255, 0, 255);
    leds.commit(); // all three go purple at once
    sleep_ms(1000);

    // Walking rainbow on remaining LEDs (hue mapped to just these 6)
    int remaining_leds[] = {6, 7, 8, 9, 10, 11};
    for (int i = 0; i < 6; i++) {
        leds.set_hsv(remaining_leds[i], (360.0f / 6) * i, 1.0f, 1.0f);
        leds.commit();
        sleep_ms(200);
    }
    sleep_ms(2000);

    // 3. Clear all LEDs
    leds.clear_all();
    sleep_ms(1000);
}

void run_spirit_level(LEDS &leds, LIS3DH &accel) { 
    // Test lis3dh driver over serial port
    AccelData raw_data = accel.read_accel_data(); // reade raw integer readings from accelerometer into a struct
    float x_g = accel.accel_data_to_g(raw_data.x); // convert raw x to g-forces
    float y_g = accel.accel_data_to_g(raw_data.y); // convert raw y to g-forces
    float z_g = accel.accel_data_to_g(raw_data.z); // convert raw z to g-forces
    printf("x: %.2fg, y: %.2fg, z: %.2fg\n", x_g, y_g, z_g); // print in serial for debugging
    sleep_ms(100);

    float magnitude = sqrt(x_g * x_g + y_g * y_g); // allows for reduced sensitivity (i.e., deadzone before leds kick in)

    if (magnitude < 0.15f) { // when board is approximately level
        int level_leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // all 12 LEDs 
        leds.set_multiple_hsv(level_leds, 12, 120.0f, 1, 0.5); // set to half brightness
    } else { 
        if (abs(x_g) > abs(y_g)) { // assuming usb is orientated to the left (i.e., blank led panel left side)
            if (x_g > 0) { // x-axis is dominant (tilting away)
                int bottom_leds[] = {8, 9, 10, 11}; // light up bottom panel
                leds.set_multiple_hsv(bottom_leds, 4, 240.0f, 1.0f, 0.5f); // light up blue - half brightness
            } else { 
                int top_leds[] = {0, 1, 2, 3}; // light up the top panel if board is tilted in the negative x-axis
                leds.set_multiple_hsv(top_leds, 4, 240.0f, 1.0f, 0.5f); // light up blue - half brightness
            }
        } else { 
            if(y_g < 0) { // y-axis is dominant 
                int corner_leds[] = {0, 11}; // tilting the board right
                leds.set_multiple_hsv(corner_leds,2, 240.0f, 1.0f, 0.5f); // corner LEDs light up blue
            } else { 
                int right_leds[] = {4, 5, 6, 7}; // tilting the board left
                leds.set_multiple_hsv(right_leds, 4, 240.0f, 1.0f, 0.5f); // right panel lights up blue
            }   
        }
    }
    leds.commit(); 
    leds.clear_all(); 
}

int main()
{
    stdio_init_all();

    LEDS leds; // create instance of LEDS class
    leds.init(12, LED_PIN); // initialise the LED drier (Extension 5. Allow the user to customise the number of LEDs on the board instead of having it hardcoded (default 12))

    LIS3DH accel; // create instance of LIS3DH class
    accel.init(); // initialise the accelerometer driver 

    int mode = 0; // tracks the current demo mode (0 = LED Driver; 1 = spirit level)
    bool last_button_state = false; // stores previous button state

    for (;;) {
        // Test the log system
        log(LogLevel::INFORMATION, "Hello world");  
        
        // Allow the SW1 to toggle between different test scenarios
        bool current_button_state = gpio_get(SW1_PIN); // read current button state
        if (current_button_state && !last_button_state) { 
            mode = (mode + 1) % 2; // cycle between mode 0 and 1
            printf("MODE: %d\n", mode); // print mode to serial for debugging
        }
        last_button_state = current_button_state; // store state for next iteration

        switch(mode) { 
            case 0: 
                // Test the LED Driver
                run_short_leds_test(leds); 
                break; 
            case 1: 
                // Test the spirit level program (lis3dh Driver)
                run_spirit_level(leds, accel); 
                break; 
        }
        sleep_ms(50); // debounce
    }
    return 0;
}