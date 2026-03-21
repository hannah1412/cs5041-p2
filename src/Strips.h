#pragma once

#include <FastLED.h>

struct CylonAnimation
{
    int pos;
    int direction;
    bool reflect;
    int fadeRate;
    unsigned long lastUpdate;
    unsigned long refreshInterval;
    CRGB color;
};