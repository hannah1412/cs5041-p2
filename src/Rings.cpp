#include <Arduino.h>
#include <FastLED.h>

#include "Config.h"
#include "Rings.h"

// temperature visuals
const WavePattern COLD = {10, CRGB::Aqua};
const WavePattern T_OPTIMAL = {60, CRGB::Green};
const WavePattern HOT = {20, CRGB::Purple};

// moisture visuals
const WavePattern DRY = {20, CRGB::Orange};
const WavePattern W_OPTIMAL = {60, CRGB::Green};
const WavePattern WET = {40, CRGB::Blue};

// removed state
const WavePattern REMOVED = {20, CRGB::Red};

void ringAnimation(CRGB *leds, RingAnimation *r)
{
    uint8_t brightness = beatsin8(r->beatsPerMinute, r->minBrightness, r->maxBrightness);
    CRGB dimmed = r->color;
    dimmed.nscale8(brightness);
    fill_solid(&leds[r->offset], r->nLeds, dimmed);
}

void apply(RingAnimation *ring, WavePattern w)
{
    ring->color = blend(ring->color, w.color, 10);
    ring->beatsPerMinute = w.freq;
}

WavePattern combine(WavePattern w1, WavePattern w2)
{
    return WavePattern{
        (w1.freq + w2.freq) / 2,
        blend(w1.color, w2.color, 128)};
}

WavePattern getWavePattern(bool mush, float temperature, float moisture)
{
    if (!mush)
        return REMOVED;

    WavePattern temp;
    WavePattern moist;

    if (temperature >= 0 && temperature < 13)
    { // cold
        temp = COLD;
    }
    else if (temperature >= 13 && temperature < 24)
    {
        temp = T_OPTIMAL;
    }
    else if (temperature >= 24 && temperature < 30)
    {
        temp = HOT;
    }

    if (moisture < 70)
    {
        moist = DRY;
    }
    else if (moisture >= 70 && moisture < 90)
    {
        moist = W_OPTIMAL;
    }
    else
    {
        moist = WET;
    }

    return combine(temp, moist);
}

void updateRingState(RingAnimation *ring, bool mush, float temperature, float moisture)
{
    WavePattern w = getWavePattern(mush, temperature, moisture);
    apply(ring, w);
}
