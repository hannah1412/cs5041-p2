#pragma once

constexpr uint8_t TEMP_MAX = 40;
constexpr uint8_t TEMP_MIN = 0;
constexpr uint8_t MOISTURE_MAX = 100;
constexpr uint8_t MOISTURE_MIN = 50;

struct MushroomSensors
{
    bool m1;
    bool m2;
    bool m3;
};

// reads the potentiometer and returns an exponential moving average reading mapped from 0->40
float readTemperature();

// reads the potentiometer and returns an exponential moving average reading mappd from 0->100.
float readMoisture();

void mushroomDetection(MushroomSensors &sensors);
