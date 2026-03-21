#include <Arduino.h>
#include "Config.h"
#include "Sensors.h"

float alpha = 0.2; // constant for exponential moving average

float readTemperature()
{
    float t_reading = analogRead(TEMP_POT_PIN);
    static float t_avg = t_reading;
    t_avg = alpha * t_reading + (1 - alpha) * t_reading;
    return map(t_avg, 0, 1023, 40, 0);
}

float readMoisture()
{
    float m_reading = analogRead(MOISTURE_POT_PIN);
    static float m_avg = m_reading;
    m_avg = alpha * m_reading + (1 - alpha) * m_reading;
    return map(m_avg, 0, 1023, 100, 0);
}

// function that reads data from LDR sensors
void mushroomDetection(MushroomSensors &sensors)
{
    // check the voltage levels on all LDRs and determine whether it's covered or not

    int m1_reading = analogRead(LDR_PIN_1);
    int m2_reading = analogRead(LDR_PIN_2);
    int m3_reading = analogRead(LDR_PIN_3);

    static int m1_avg = m1_reading;
    static int m2_avg = m2_reading;
    static int m3_avg = m3_reading;

    // update with an exponentially weighted moving average
    m1_avg = alpha * m1_reading + (1 - alpha) * m1_avg;
    m2_avg = alpha * m2_reading + (1 - alpha) * m2_avg;
    m3_avg = alpha * m3_reading + (1 - alpha) * m3_avg;

    // check thresholds
    sensors.m1 = m1_avg < LDR_THRESHOLD_EXPOSED;
    sensors.m2 = m2_avg < LDR_THRESHOLD_EXPOSED;
    sensors.m3 = m3_avg < LDR_THRESHOLD_EXPOSED;
}