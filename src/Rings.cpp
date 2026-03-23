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
    if(r -> nLedsActivated <0){
        uint8_t brightness = beatsin8(r->beatsPerMinute, r->minBrightness, r->maxBrightness);
        CRGB dimmed = r->color;
        dimmed.nscale8(brightness);
        fill_solid(&leds[r->offset], r->nLeds, dimmed);
    }else{
        fill_solid(&leds[r->offset], r->nLeds, CRGB::Black);
        if(r -> nLedsActivated > 0){
            CRGB c = r -> color;
            c.nscale8(r->maxBrightness);
            fill_solid(&leds[r->offset], r->nLedsActivated, c);
        }
    }
}

// Blending colors 
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

// -----------------------cal health score - combination of both TEMP + MOIST -----------------
static int calHealth(float temp, float moist){
    int tempScore;
    int moistScore;
    if(temp >= 13 && temp < 24){
        tempScore =50;  //optimal 
    }else if(temp >= 0 && temp <13){
        tempScore = (int)map((long) temp, 0, 13, 0, 40);    //cold
    }else{
        tempScore = (int)map((long) constrain(temp, 24, 30), 24, 30, 40, 0);    //too hot 
    }

    if(moist >= 70 && moist < 90){
        moistScore =50;  //optimal 
    }else if(moist >= 0 && moist <13){
        moistScore = (int)map((long) moist, 50, 70, 0, 40);    //dry
    }else{
        moistScore = (int)map((long) constrain(moist, 90, 100), 90, 100, 40, 0);    //wet
    }

    return tempScore + moistScore;
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



void updateRingState(RingAnimation *ring, bool mush, float temperature, float moisture, int vitality)
{
    // max birghtness level when network strongest === 3 mushs present
    const uint8_t v_birghtness_level[4] = {5, 40, 60, 255};
    ring-> maxBrightness = v_birghtness_level[constrain(vitality, 0, 3)];
    ring -> minBrightness = ring -> maxBrightness/8;
    // no Mush 
    if(!mush){
        apply(ring,REMOVED);
        ring -> nLedsActivated = -1;
    }else{
        WavePattern w = getWavePattern(mush, temperature, moisture);
        apply(ring, w);

        int health = calHealth(temperature, moisture);
        // map health dynamically 
        ring -> nLedsActivated = (int)map(health, 0, 100, 1, ring-> nLeds);
    }
    
}
