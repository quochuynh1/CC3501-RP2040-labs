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
#include "drivers/microphone.h"
#include "arm_math.h"

// Define Global Variables 
int16_t hanning_window[1024] {
    0,0,1,3,5,8,11,15,20,25,31,37,44,52,61,69,79,89,100,111,123,136,149,163,178,193,208,225,242,259,277,296,315,335,356,377,399,421,444,468,492,517,542,568,595,622,650,678,707,736,767,797,829,860,893,926,960,994,1029,1064,1100,1137,1174,1211,1250,1288,1328,1368,1408,1449,1491,1533,1576,1619,1663,1708,1753,1798,1844,1891,1938,1986,2034,2083,2133,2182,2233,2284,2335,2387,2440,2493,2547,2601,2656,2711,2766,2823,2879,2937,2994,3053,3111,3171,3230,3291,3351,3413,3474,3536,3599,3662,3726,3790,3855,3920,3985,4051,4118,4185,4252,4320,4388,4457,4526,4596,4666,4737,4808,4879,4951,5023,5096,5169,5243,5317,5391,5466,5541,5617,5693,5769,5846,5923,6001,6079,6158,6236,6316,6395,6475,6555,6636,6717,6799,6880,6962,7045,7128,7211,7295,7379,7463,7547,7632,7717,7803,7889,7975,8062,8148,8236,8323,8411,8499,8587,8676,8765,8854,8944,9033,9123,9214,9304,9395,9486,9578,9670,9761,9854,9946,10039,10132,10225,10318,10412,10505,10599,10694,10788,10883,10978,11073,11168,11264,11359,11455,11551,11648,11744,11841,11937,12034,12131,12229,12326,12424,12521,12619,12717,12815,12914,13012,13111,13209,13308,13407,13506,13605,13704,13804,13903,14003,14102,14202,14302,14401,14501,14601,14701,14802,14902,15002,15102,15203,15303,15403,15504,15604,15705,15806,15906,16007,16107,16208,16309,16409,16510,16610,16711,16812,16912,17013,17113,17214,17314,17415,17515,17616,17716,17816,17916,18017,18117,18217,18317,18416,18516,18616,18716,18815,18915,19014,19113,19213,19312,19411,19509,19608,19707,19805,19904,20002,20100,20198,20296,20393,20491,20588,20685,20782,20879,20976,21072,21169,21265,21361,21457,21552,21647,21743,21838,21932,22027,22121,22216,22309,22403,22497,22590,22683,22776,22868,22961,23053,23144,23236,23327,23418,23509,23599,23690,23780,23869,23959,24048,24136,24225,24313,24401,24489,24576,24663,24750,24836,24922,25008,25093,25178,25263,25347,25431,25515,25599,25682,25764,25847,25929,26010,26091,26172,26253,26333,26413,26492,26571,26650,26728,26806,26883,26960,27037,27113,27189,27265,27340,27414,27488,27562,27636,27708,27781,27853,27925,27996,28067,28137,28207,28276,28345,28414,28482,28550,28617,28683,28750,28815,28881,28946,29010,29074,29137,29200,29263,29325,29386,29447,29508,29568,29627,29686,29745,29803,29860,29917,29974,30029,30085,30140,30194,30248,30301,30354,30407,30458,30510,30560,30611,30660,30709,30758,30806,30853,30900,30947,30993,31038,31083,31127,31170,31213,31256,31298,31339,31380,31420,31460,31499,31538,31576,31613,31650,31686,31722,31757,31791,31825,31859,31891,31924,31955,31986,32017,32046,32076,32104,32132,32160,32187,32213,32239,32264,32288,32312,32335,32358,32380,32402,32422,32443,32462,32481,32500,32518,32535,32551,32567,32583,32598,32612,32625,32638,32651,32662,32673,32684,32694,32703,32712,32720,32727,32734,32740,32746,32751,32755,32759,32762,32764,32766,32767,32767,32767,32767,32766,32764,32762,32759,32755,32751,32746,32740,32734,32727,32720,32712,32703,32694,32684,32673,32662,32651,32638,32625,32612,32598,32583,32567,32551,32535,32518,32500,32481,32462,32443,32422,32402,32380,32358,32335,32312,32288,32264,32239,32213,32187,32160,32132,32104,32076,32046,32017,31986,31955,31924,31891,31859,31825,31791,31757,31722,31686,31650,31613,31576,31538,31499,31460,31420,31380,31339,31298,31256,31213,31170,31127,31083,31038,30993,30947,30900,30853,30806,30758,30709,30660,30611,30560,30510,30458,30407,30354,30301,30248,30194,30140,30085,30029,29974,29917,29860,29803,29745,29686,29627,29568,29508,29447,29386,29325,29263,29200,29137,29074,29010,28946,28881,28815,28750,28683,28617,28550,28482,28414,28345,28276,28207,28137,28067,27996,27925,27853,27781,27708,27636,27562,27488,27414,27340,27265,27189,27113,27037,26960,26883,26806,26728,26650,26571,26492,26413,26333,26253,26172,26091,26010,25929,25847,25764,25682,25599,25515,25431,25347,25263,25178,25093,25008,24922,24836,24750,24663,24576,24489,24401,24313,24225,24136,24048,23959,23869,23780,23690,23599,23509,23418,23327,23236,23144,23053,22961,22868,22776,22683,22590,22497,22403,22309,22216,22121,22027,21932,21838,21743,21647,21552,21457,21361,21265,21169,21072,20976,20879,20782,20685,20588,20491,20393,20296,20198,20100,20002,19904,19805,19707,19608,19509,19411,19312,19213,19113,19014,18915,18815,18716,18616,18516,18416,18317,18217,18117,18017,17916,17816,17716,17616,17515,17415,17314,17214,17113,17013,16912,16812,16711,16610,16510,16409,16309,16208,16107,16007,15906,15806,15705,15604,15504,15403,15303,15203,15102,15002,14902,14802,14701,14601,14501,14401,14302,14202,14102,14003,13903,13804,13704,13605,13506,13407,13308,13209,13111,13012,12914,12815,12717,12619,12521,12424,12326,12229,12131,12034,11937,11841,11744,11648,11551,11455,11359,11264,11168,11073,10978,10883,10788,10694,10599,10505,10412,10318,10225,10132,10039,9946,9854,9761,9670,9578,9486,9395,9304,9214,9123,9033,8944,8854,8765,8676,8587,8499,8411,8323,8236,8148,8062,7975,7889,7803,7717,7632,7547,7463,7379,7295,7211,7128,7045,6962,6880,6799,6717,6636,6555,6475,6395,6316,6236,6158,6079,6001,5923,5846,5769,5693,5617,5541,5466,5391,5317,5243,5169,5096,5023,4951,4879,4808,4737,4666,4596,4526,4457,4388,4320,4252,4185,4118,4051,3985,3920,3855,3790,3726,3662,3599,3536,3474,3413,3351,3291,3230,3171,3111,3053,2994,2937,2879,2823,2766,2711,2656,2601,2547,2493,2440,2387,2335,2284,2233,2182,2133,2083,2034,1986,1938,1891,1844,1798,1753,1708,1663,1619,1576,1533,1491,1449,1408,1368,1328,1288,1250,1211,1174,1137,1100,1064,1029,994,960,926,893,860,829,797,767,736,707,678,650,622,595,568,542,517,492,468,444,421,399,377,356,335,315,296,277,259,242,225,208,193,178,163,149,136,123,111,100,89,79,69,61,52,44,37,31,25,20,15,11,8,5,3,1,0,0};

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

void run_microphone_test(MICROPHONE &mic) { 
    uint16_t adc_data[256]; // declare an array of 256 unsigned 16-bit integers
    mic.read(adc_data, 256); // buffer (adc_data) contains 256 raw ADC samples, each one a 12-bit value between 0, and 4095

    // Calculate the average ADC value  
    uint32_t sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += adc_data[i];
    }
    float mean = sum / 256.0f;

    // Use the serial port to inspect the numbers and confirm that they tend to oscillate around the expected value
    // Note: DC bias at Vcc/2 = 3.3V/2 = 1.65V; therefore, expected_mean = (1.65/3.3) * 4095 = 2047
    printf("Mean ADC: %.1f\n", mean); 
    sleep_ms(500); 
}

void run_simple_mic_app(LEDS &leds, MICROPHONE &mic, arm_rfft_instance_q15 &fft_instance) {
    // 1. Read 1024 samples from the microphone into a buffer of type uint16_t
    uint16_t adc_data[1024]; 
    mic.read(adc_data, 1024); 

    // 2. Calculate the DC bias
    int32_t sum = 0; // can't use int16_t because 1024 12-bit ADC will overflow
    for (int i = 0; i < 1024; i++) { 
        sum += adc_data[i]; 
    }
    int32_t dc_bias = sum/1024; 

    // 3. Construct a new int16_t array to contain the time domain signal
    int16_t time_domain[1024]; 
    for (int i = 0; i < 1024; i++) { 
        time_domain[i] = adc_data[i] - dc_bias; // fill in each element in the buffer by taking the raw ADC values and subtracting off the DC bias
        
        // Note: lab instructions said to left shift << 3, or << 5 for higher precision, however Luke later put 
        // out an announcement saying that "simply left-shifting is likely to overflow the representation" 
        
        // Leaving out the bit-shift means that the output is not in Q15 format however, Luke made the point 
        // that it is better practice to feed the output of the rfft directly into the input of the complex_mag 
        // function. This way, you can perform operations on the output directly, or change their representaiton 
        // after all the operations are complete. It is unlikely to affect the mapping of the frequency bins to the LEDs,
    }

    // 4. Constants stored in Glbal variable at the top - generated by MATLAB
    // Apply the window function to your measured samples - the operation is an element-wise multiplication 
    // (i.e., for each element in the input, multiply by the corresponding element in the Hanning window)
    for (int i = 0; i < 1024; i++) { 
        // See Lecture 4 "Multiplication in Fixed Point"
        time_domain[i] = ((int32_t)time_domain[i] * (int32_t)hanning_window[i]) >> 15; // need to cast both up to int32_t to avoid overflow as well as right shift to as discussed in lecture notes
        time_domain[i] = (int16_t)time_domain[i];  // then cast back to int16_t
    }

    // 5. Calculate the FFT using the arm_rfft_q15 function
    int16_t fft_output[1024]; // declare a buffer to hold the results aftering running the fft 
    arm_rfft_q15(&fft_instance, time_domain, fft_output); // function perameters: address of the struct fft_instance (passed in as a pointer); pointer to the time_domain[] buffer; pointer to the fft_output buffer (declared above)

    // 6. Calculate the Energy Spectral Density of the audio signal 
    int16_t mag_squared_values[512]; // declare a buffer to hold the magnitude squared values (512 because one magnitude sample per frequency bin (i.e., half the total samples))
    arm_cmplx_mag_squared_q15(fft_output, mag_squared_values, 512); // function parameters: a pointer to the fft_output buffer, pointer to the mag_squared_values buffer, and number of samples = 512

    // 7. Visually display the results on the LEDs (simple version)
    int32_t magnitude_sum = 0; 
    for (int i = 0; i < 512; i++) {
        magnitude_sum += mag_squared_values[i]; 
    }
    
    printf("magnitude_sum: %d\n", magnitude_sum); // debug line

    if (magnitude_sum >= 1) { 
        leds.set_all(0, 0, 255); 
        leds.commit(); 
    } else { 
        leds.clear_all(); 
    }
}

void run_advanced_mic_app(LEDS &leds, MICROPHONE &mic, arm_rfft_instance_q15 &fft_instance) { 
    // 1. Read 1024 samples from the microphone into a buffer of type uint16_t
    uint16_t adc_data[1024]; 
    mic.read(adc_data, 1024); 

    // 2. Calculate the DC bias
    int32_t sum = 0; // can't use int16_t because 1024 12-bit ADC will overflow
    for (int i = 0; i < 1024; i++) { 
        sum += adc_data[i]; 
    }
    int32_t dc_bias = sum/1024; 

    // 3. Construct a new int16_t array to contain the time domain signal
    int16_t time_domain[1024]; 
    for (int i = 0; i < 1024; i++) { 
        time_domain[i] = adc_data[i] - dc_bias; // fill in each element in the buffer by taking the raw ADC values and subtracting off the DC bias
        
        // Note: lab instructions said to left shift << 3, or << 5 for higher precision, however Luke later put 
        // out an announcement saying that "simply left-shifting is likely to overflow the representation" 
        
        // Leaving out the bit-shift means that the output is not in Q15 format however, Luke made the point 
        // that it is better practice to feed the output of the rfft directly into the input of the complex_mag 
        // function. This way, you can perform operations on the output directly, or change their representaiton 
        // after all the operations are complete. It is unlikely to affect the mapping of the frequency bins to the LEDs,
    }

    // 4. Constants stored in Glbal variable at the top - generated by MATLAB
    // Apply the window function to your measured samples - the operation is an element-wise multiplication 
    // (i.e., for each element in the input, multiply by the corresponding element in the Hanning window)
    for (int i = 0; i < 1024; i++) { 
        // See Lecture 4 "Multiplication in Fixed Point"
        time_domain[i] = ((int32_t)time_domain[i] * (int32_t)hanning_window[i]) >> 15; // need to cast both up to int32_t to avoid overflow as well as right shift to as discussed in lecture notes
        time_domain[i] = (int16_t)time_domain[i];  // then cast back to int16_t
    }

    // 5. Calculate the FFT using the arm_rfft_q15 function
    int16_t fft_output[1024]; // declare a buffer to hold the results aftering running the fft 
    arm_rfft_q15(&fft_instance, time_domain, fft_output); // function perameters: address of the struct fft_instance (passed in as a pointer); pointer to the time_domain[] buffer; pointer to the fft_output buffer (declared above)

    // 6. Calculate the Energy Spectral Density of the audio signal 
    int16_t mag_squared_values[512]; // declare a buffer to hold the magnitude squared values (512 because one magnitude sample per frequency bin (i.e., half the total samples))
    arm_cmplx_mag_squared_q15(fft_output, mag_squared_values, 512); // function parameters: a pointer to the fft_output buffer, pointer to the mag_squared_values buffer, and number of samples = 512

    // 7. Visually display the results on the LEDs (simple version)
    // The FFT bin indices that delimit betwen LEDs are logorithmically spaced
    // In MATLAB: >> ceil(logspace(log10(5), log10(512), 13)) (Code provided by lab instructions)
    int frequency_bin_boundaries[] = {6, 8, 11, 16, 24, 35, 51, 75, 110, 161, 237, 349, 513}; // used MATLAB to create 13 boundaries (i.e., 12 bands - 1 for each LED)
    // Note: mapping is as follows (i.e., the [8, 11) notation used in the lab instructions)
    // [band 0]: 6 -> 8
    // [band 1]: 8 -> 11
    // [band 2]: 11 -> 16 

    leds.clear_all(); 
    // Determine the total energy for each band
    int max_energy = 3; 
    for (int band = 0; band < NUM_LEDS; band++) { 
        int32_t band_energy = 0; // variable will hold the sum of the magnitude_squared_values (i.e., total energy spectral density) for that particular band
        for (int i = frequency_bin_boundaries[band]; i < frequency_bin_boundaries[band + 1]; i++) { 
            band_energy += mag_squared_values[i]; 
        }
        
        printf("Band %d: %d\n", band, band_energy); // debug line 

        // Now, if the band reaches a threshold of 1 (no bit shifting), turn on the corresponding LED
        // LED bands are broken up into different hues, as well as can change brightness depending on the total energy within that band
        if(band_energy >= 1) {
            float brightness = (float)band_energy/max_energy; 
            leds.set_hsv(band, (band * 360.0)/12, 1, brightness); 
        }
    }
    leds.commit(); 
}

int main()
{
    stdio_init_all();

    LEDS leds; // create instance of LEDS class
    leds.init(12, LED_PIN); // initialise the LED drier (Extension 5. Allow the user to customise the number of LEDs on the board instead of having it hardcoded (default 12))

    LIS3DH accel; // create instance of LIS3DH class
    accel.init(); // initialise the accelerometer driver 

    MICROPHONE mic; // create instance of MICROPHONE class
    // Once at the start of the function/main: 
    // 1. Inisialise the microphone (using your driver that you wrote earlier)
    mic.init(); // initialise the microphone driver

    // 2. Use the arm_rfft_init_q15 function to intialise the FFT instance
    arm_rfft_instance_q15 fft_instance; // declare a struct for the fft info to be stored in
    arm_rfft_init_q15(&fft_instance, 1024, 0, 1); // parameters: address of the struct fft_instance (passed in as a pointer); number of samples = 1024; 0 = forward FFT; 1 = enable bit-reversal 


    int mode = 0; // tracks the current demo mode (0 = LED Driver; 1 = spirit level)
    bool last_button_state = false; // stores previous button state

    for (;;) {
        // Test the log system
        log(LogLevel::INFORMATION, "Hello world");  
        
        // Allow the SW1 to toggle between different test scenarios
        bool current_button_state = gpio_get(SW1_PIN); // read current button state
        int number_of_modes = 3; 
        if (current_button_state && !last_button_state) { 
            mode = (mode + 1) % number_of_modes; // cycle between mode 0 and 1
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
            case 2: 
                // Test the Microphone Driver
                // run_microphone_test(mic); 

                // Test the simple microphone application
                run_advanced_mic_app(leds, mic, fft_instance); 
                break; 
        }
        sleep_ms(100); // debounce
    }
    return 0;
}