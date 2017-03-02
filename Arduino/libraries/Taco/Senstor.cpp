#include <Sensor.h>

float Sensor::update(float value)
{
    prevVal = curVal;
    curVal = value
    return value;
}
