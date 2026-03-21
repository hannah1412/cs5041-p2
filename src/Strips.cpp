// CRGBSet stripSet[NUM_STRIPS] = {
//     CRGBSet(strips, 0, LEDS_PER_STRIP - 1),
//     CRGBSet(strips, LEDS_PER_STRIP, 2 * LEDS_PER_STRIP - 1),
//     CRGBSet(strips, 2 * LEDS_PER_STRIP, 3 * LEDS_PER_STRIP - 1),
// };

// // -- code adapted from fastled cookbook github
// void cylon(CRGB *leds, int NUM_LEDS, CylonAnimation *state)
// {
//     long currMillis = millis();

//     if (currMillis - state->lastUpdate >= state->refreshInterval)
//     {
//         // Fade all LEDs
//         fadeToBlackBy(leds, NUM_LEDS, state->fadeRate);
//         leds[state->pos] = state->color;

//         // update the position and reflect if needed
//         int newPos = state->pos + state->direction;

//         if (newPos < 0 || newPos >= NUM_LEDS)
//         {
//             if (state->reflect)
//             {
//                 state->direction = -state->direction;
//                 newPos = state->pos + state->direction;
//             }
//             else
//             {
//                 newPos = state->pos - NUM_LEDS * state->direction + state->direction;
//             }
//         }

//         // update position
//         state->pos = newPos;
//         state->lastUpdate = currMillis;
//     }
// }