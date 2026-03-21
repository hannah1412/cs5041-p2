#pragma once
#include <FastLED.h>
#include <Arduino.h>

// heartbeat pattern
struct RingAnimation
{
    int beatsPerMinute;
    int maxBrightness;
    int minBrightness;
    int nLeds;
    int offset;
    CRGB color;
};

void ringAnimation(CRGB *leds, RingAnimation *state);

void updateRingState(RingAnimation *ring, bool mush, float temperature, float moisture);