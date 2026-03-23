#include <Arduino.h>
#include <FastLED.h>
#include "Strips.h"
#include "Sensors.h"
#include "Config.h"

void updateStripState(StripAnimation *s1, StripAnimation *s2, StripAnimation *s3, MushroomSensors mush, int vitality)
{
    const uint8_t v_bright[4] = {10, 50, 70, 255};
    const uint8_t v_BPM[4] = {8, 16, 24, 40};
    uint8_t bright = v_bright[constrain(vitality, 0, 3)];
    uint8_t bpm = v_bright[constrain(vitality, 0, 3)];

    s1-> maxBrightness = bright; 
    s1-> pulseBpm = bpm;
    s2-> maxBrightness = bright; 
    s2-> pulseBpm = bpm;
    s3-> maxBrightness = bright; 
    s3-> pulseBpm = bpm;


    int currTime = millis();

    // perform an update at regular intervals
    if (currTime - s1->lastUpdate >= s1->refreshInterval)
    {
        if (mush.m1 && mush.m2)
        { // path connected between
            int activated = s1->nLedsActivated;
            s1->nLedsActivated = constrain(activated + 1, 0, s1->nLeds);
        }

        if (mush.m2 && mush.m3)
        {
            int n2 = s2->nLedsActivated;
            int n3 = s3->nLedsActivated;
            s2->nLedsActivated = constrain(n2 + 1, 0, s2->nLeds);
            s3->nLedsActivated = constrain(n3 + 1, 0, s3->nLeds);
        }
        else
        {
            int n2 = s2->nLedsActivated;
            int n3 = s3->nLedsActivated;
            s2->nLedsActivated = constrain(n2 - 1, 0, s2->nLeds);
            s3->nLedsActivated = constrain(n3 - 1, 0, s3->nLeds);
        }

        if (!mush.m1)
        {
            int activated = s1->nLedsActivated;
            s1->nLedsActivated = constrain(activated - 1, 0, s1->nLeds);
        }
        s1->lastUpdate = currTime;
        s2->lastUpdate = currTime;
        s3->lastUpdate = currTime;
    }
};

void stripAnimation(CRGB *leds, StripAnimation *s)
{
    // turn off LEDs out of nLedActivated range 
    fill_solid(&leds[s->offset], s -> nLeds, CRGB::Black);
    if(s -> nLedsActivated  <=0) return ;


    // 1 full pass every 2 secs
    uint8_t t = beat8(30);
    int nActive = s -> nLedsActivated;
    const int TRAIL_LEN = 4;

    float headF = (t/255.0f)*nActive;
    int head = (int)headF;
    
    for(int trail =0; trail < TRAIL_LEN; trail++){
        int ledPos = head - trail;

        // wrap s.t. the leds at the end of each strip stays visible
        if(ledPos < 0){
            ledPos += nActive;
        }
        if(ledPos >= nActive){
            continue;
        }

        //  set up brightness level -> distinguish the different broightness level between begin VS end of each light strip
        uint8_t brightness = (uint8_t)map(trail, 0, TRAIL_LEN -1,s -> maxBrightness, s-> minBrightness);
        CRGB c = s-> color;
        c.nscale8(brightness);
        // if strip runs in reverse -> flip index
        int idx = s -> reversed ? (s-> nLeds -1 -ledPos): ledPos;
        leds[s-> offset +idx] =c;
    }
}