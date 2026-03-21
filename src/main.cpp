#include <Arduino.h>
#include <FastLED.h>

// custom imports
#include "Rings.h"
#include "Sensors.h"
#include "Strips.h"
#include "Config.h"

// CRGB object for FastLED library

CRGB strips[NUM_STRIPS * LEDS_PER_STRIP + 2];
CRGB rings[NUM_RINGS * LEDS_PER_RING + 2];

RingAnimation ring1;
RingAnimation ring2;
RingAnimation ring3;

// treat as one long strip
// CylonAnimation strip1;

MushroomSensors sensors;

void setup()
{
    Serial.begin(9600);
    // set up rings
    FastLED.addLeds<WS2812, RINGS_LED_PIN, GRB>(rings, NUM_RINGS * LEDS_PER_RING + 2).setRgbw(RgbwDefault());

    // set up strips
    FastLED.addLeds<WS2812, STRIPS_LED_PIN, GRB>(strips, NUM_STRIPS * LEDS_PER_STRIP + 2).setRgbw(RgbwDefault());

    // limit overall brightness
    // FastLED.setBrightness(150);
    // strip1 = {0, 1, false, 20, 0, 500, CRGB::White};

    ring1 = {20, 150, 5, LEDS_PER_RING, 0, CRGB::White};
    ring2 = {20, 150, 5, LEDS_PER_RING, 16, CRGB::White};
    ring3 = {20, 150, 5, LEDS_PER_RING, 32, CRGB::White};

    sensors = {false,
               false,
               false};
}

// sensor data from toggles
float temperature = 0; // range from 0 to 40 C
float moisture = 0;    // range from 0 to 100 %

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
// void updateStrips()
// {
//     // strips line up between m1 -> m2 -> <- m3;
//     if (ring1.mushroomPresence && ring2.mushroomPresence && ring3.mushroomPresence)
//     { // send pulse down strip 1
//         cylon(strips, NUM_STRIPS * LEDS_PER_STRIP, &strip1);
//     }
// }

void loop()
{

    EVERY_N_MILLISECONDS(10)
    { // approx 100 FPS
        mushroomDetection(sensors);
        temperature = readTemperature();
        moisture = readMoisture();

        // updateStrips();
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