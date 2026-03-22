#include <Arduino.h>
#include <FastLED.h>

// custom imports
#include "Rings.h"
#include "Sensors.h"
#include "Strips.h"
#include "Config.h"

// CRGB object for FastLED library

CRGB strips[NUM_STRIPS * LEDS_PER_STRIP];
CRGB rings[NUM_RINGS * LEDS_PER_RING + (NUM_STRIPS * LEDS_PER_STRIP)];

RingAnimation ring1;
RingAnimation ring2;
RingAnimation ring3;

StripAnimation strip1;
StripAnimation strip2;
StripAnimation strip3;

MushroomSensors sensors;
float temperature = 0; // range from 0 to 40 C
float moisture = 0;    // range from 0 to 100 %

void setup()
{
    Serial.begin(9600);
    // set up rings
    FastLED.addLeds<WS2812, RINGS_LED_PIN, GRB>(rings, NUM_RINGS * LEDS_PER_RING + (NUM_STRIPS * LEDS_PER_STRIP)).setRgbw(RgbwDefault());

    // limit overall brightness
    // FastLED.setBrightness(150);
    strip1 = {false, 10, 0, 500, CRGB::White, 150, 0, LEDS_PER_STRIP, 48, 0};
    strip2 = {false, 10, 0, 500, CRGB::White, 150, 0, LEDS_PER_STRIP, 56, 0};
    strip3 = {true, 10, 0, 500, CRGB::White, 150, 0, LEDS_PER_STRIP, 64, 0};

    ring1 = {20, 150, 10, LEDS_PER_RING, 0, CRGB::White};
    ring2 = {20, 150, 10, LEDS_PER_RING, 16, CRGB::White};
    ring3 = {20, 150, 10, LEDS_PER_RING, 32, CRGB::White};

    sensors = {false,
               false,
               false};
}

// animations for the mushroom rings
void updateRings()
{
    // update color based on presence of mushrooms
    updateRingState(&ring1, sensors.m1, temperature, moisture);
    updateRingState(&ring2, sensors.m2, temperature, moisture);
    updateRingState(&ring3, sensors.m3, temperature, moisture);

    ringAnimation(rings, &ring1);
    ringAnimation(rings, &ring2);
    ringAnimation(rings, &ring3);
}

// animations for the micellium paths (strips)
void updateStrips()
{
    // strips line up between m1 -> m2 -> <- m3;
    updateStripState(&strip1, &strip2, &strip3, sensors);

    stripAnimation(rings, &strip1); // test on rings
    stripAnimation(rings, &strip2);
    stripAnimation(rings, &strip3);
}

void loop()
{

    EVERY_N_MILLISECONDS(20)
    { // approx 50 FPS
        mushroomDetection(sensors);
        temperature = readTemperature();
        moisture = readMoisture();

        updateStrips();
        updateRings();

        FastLED.show();
    }

    EVERY_N_MILLISECONDS(1500)
    {
        Serial.print("Temperature: ");
        Serial.println(temperature);
        Serial.print("Moisture: ");
        Serial.println(moisture);
    }
}