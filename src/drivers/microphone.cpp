#include "microphone.h"
#include "hardware/adc.h"
#include "board.h"

void MICROPHONE::init() { 
    adc_init(); 

    adc_gpio_init(MIC_PIN); // GPIO26 on shcematic
    adc_select_input(0); // ADC0 on schematic

    adc_set_clkdiv(1087.435f); // calculated as part of lab 4
    adc_fifo_setup(true, false, 1, false, false); // enable FIFO, disable DMA
}

void MICROPHONE::read(uint16_t *adc_data, uint16_t number_of_samples) { // read number of ADC samples into adc_data buffer
    adc_run(true); // (a) turn on free-running sampling mode

    for (int i = 0; i < number_of_samples; i++) { 
        adc_data[i] = adc_fifo_get_blocking(); // (b) read from the FIFO buffer
    }

    adc_run(false); // (c) turn off free-running sampling mode
    adc_fifo_drain(); // (d) drain the FIFO to discard any further samples that were collected by the ADC
}