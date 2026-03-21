#pragma once

#include <Arduino.h>

// project configuration Neopixels (pins)
constexpr uint8_t RINGS_LED_PIN = 6;
constexpr uint8_t STRIPS_LED_PIN = 5;
constexpr uint8_t NUM_RINGS = 3;
constexpr uint8_t NUM_STRIPS = 3;
constexpr uint8_t LEDS_PER_RING = 16;
constexpr uint8_t LEDS_PER_STRIP = 8;

// project configuration - LDRs
constexpr uint8_t LDR_PIN_1 = A4;
constexpr uint8_t LDR_PIN_2 = A5;
constexpr uint8_t LDR_PIN_3 = A3;
constexpr int LDR_THRESHOLD_EXPOSED = 400; // less light = higher number

// project configuration - toggles
constexpr uint8_t TEMP_POT_PIN = A0;
constexpr uint8_t MOISTURE_POT_PIN = A1;