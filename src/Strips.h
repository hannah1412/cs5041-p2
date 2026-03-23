#pragma once

#include <FastLED.h>
#include "Sensors.h"

struct StripAnimation
{
    bool reversed;
    int fadeRate;

    unsigned long lastUpdate;
    unsigned long refreshInterval;
    CRGB color;

    int maxBrightness;
    int minBrightness;
    int nLeds;
    int offset;
    int nLedsActivated;
    uint8_t pulseBpm; //dependent on network's vitality 
};

void stripAnimation(CRGB *leds, StripAnimation *state);

void updateStripState(StripAnimation *s1, StripAnimation *s2, StripAnimation *s3, MushroomSensors mush, int vitality);