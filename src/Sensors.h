#pragma once

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
