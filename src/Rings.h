#pragma once
#include <FastLED.h>
#include <Arduino.h>

struct WavePattern
{
    int freq;
    CRGB color;
};

// heartbeat pattern
struct RingAnimation
{
    int beatsPerMinute;
    int maxBrightness;
    int minBrightness;
    int nLeds;
    int offset;
    CRGB color;

    // health 
    int nLedsActivated;
};

void ringAnimation(CRGB *leds, RingAnimation *state);

void updateRingState(RingAnimation *ring, bool mush, float temperature, float moisture);