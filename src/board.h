#define ACCEL_I2C_INSTANCE i2c0 // rename from I2C_INSTANCE to avoid clash with i2c.h
#define ACCEL_SCL_PIN 16 // GPIO pin 16
#define ACCEL_SDA_PIN 17 // GPIO pin 17
#define ACCEL_ADDRESS 0x19 // slave address
#define SW1_PIN 15 // GPIO pin 15
#define MIC_PIN 26 // GPIO pin 26

static const int LED_PIN = 14; // define the GPIO pin that connects to first LED in daisy chain
static const int NUM_LEDS = 12; // define how many LEDs our board has
