#include <Arduino.h>
#include <FastLED.h>

#include "Config.h"
#include "Rings.h"

void ringAnimation(CRGB *leds, RingAnimation *r)
{
    int t = beat8(r->beatsPerMinute);
    int brightness = quadwave8(t);
    brightness = scale8(brightness, r->maxBrightness - r->minBrightness);
    brightness = brightness + r->minBrightness;

    // int sharpness = quadwave8(t + r->beatsPerMinute + r->beatsPerMinute / 2);
    // sharpness = scale8(sharpness, r->maxBrightness - r->minBrightness);
    // sharpness = sharpness + r->minBrightness;

    // int total = qadd8(brightness, sharpness);
    int total = brightness;
    // apply dimming to target color and
    (r->color).nscale8(total);
    fill_solid(&leds[r->offset], r->nLeds, r->color);
}

void updateRingState(RingAnimation *ring, bool mush, float temperature, float moisture)
{

    // if no mushroom on board -> blend to a slow red
    if (!mush)
    {
        ring->color = blend(ring->color, CRGB::Red, 5);
        ring->beatsPerMinute = 20;
        return;
    }

    // temperature input
    if (temperature >= 0 && temperature < 13) // too cold for mushrooms to grow
    {
        ring->color = blend(ring->color, CRGB::Aqua, 5);
        ring->beatsPerMinute = 10;
    }
    else if (temperature >= 13 && temperature < 24) // ideal range
    {
        ring->color = blend(ring->color, CRGB::Green, 5);
        ring->beatsPerMinute = 60; // faster "alive" heartbeat
    }
    else if (temperature >= 24 && temperature < 30) // mushrooms start dying
    {
        ring->color = blend(ring->color, CRGB::Purple, 5);
        ring->beatsPerMinute = 15; // slower "dying" heartbeat
    }
    else
    {
    }

    // moisture inputs
    if (moisture < 60) // too dry
    {
        ring->color = blend(ring->color, CRGB::Purple, 5);
        ring->beatsPerMinute = 10; // slower "dying" heartbeat
    }
    else if (moisture >= 60 && moisture < 75)
    { // a bit dry
        ring->color = blend(ring->color, CRGB::Orange, 5);
        ring->beatsPerMinute = 10; // slower "dying" heartbeat
    }
    else
    {
        ring->color = blend(ring->color, CRGB::Green, 5); // back to green
        ring->beatsPerMinute = 30;                        // slow "no heartbeat" dead
    }
}
