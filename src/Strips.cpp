#include <Arduino.h>
#include <FastLED.h>
#include "Strips.h"
#include "Sensors.h"
#include "Config.h"

void updateStripState(StripAnimation *s1, StripAnimation *s2, StripAnimation *s3, MushroomSensors mush)
{
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
    uint8_t t = beat8(30);
    fadeToBlackBy(&leds[s->offset], s->nLeds, s->fadeRate);
    for (int i = 0; i < s->nLeds; i++)
    {
        // allows for directions to be changed
        int idx = s->reversed ? (s->nLeds - 1 - i) : i;
        int phase = s->reversed ? -40 : 40;
        if (i < s->nLedsActivated)
        {
            uint8_t pos = t - i * phase;
            uint8_t val = cubicwave8(pos);
            CRGB c = s->color;
            leds[s->offset + idx] = blend(c, CRGB::Blue, val);
        }
    }
}