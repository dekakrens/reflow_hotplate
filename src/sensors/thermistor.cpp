#include "config/config.h"
#include <math.h>

double readThermistor()
{
    int adc = analogRead(THERM_PIN);
    if (adc <= 0)
        return NAN;

    double resistance = SERIES_RESISTOR / ((4095.0 / adc) - 1.0);
    double steinhart = log(resistance / NOMINAL_RESISTANCE);
    steinhart /= BETA_COEFFICIENT;
    steinhart += 1.0 / (NOMINAL_TEMP + 273.15);
    steinhart = 1.0 / steinhart;
    return steinhart - 273.15;
}