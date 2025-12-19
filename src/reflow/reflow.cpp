#include "config/config.h"

unsigned long stateStart = 0;

void reflowStateChange()
{
    auto &p = profiles[activeProfile];
    unsigned long elapsed = millis() - stateStart;

    switch (state)
    {
    case PREHEAT:
    {
        double ramp = elapsed * p.preheatTemp / (p.preheatTime * 1000.0);
        setpoint = (ramp < p.preheatTemp) ? ramp : p.preheatTemp;
        if (elapsed > p.preheatTime * 1000)
        {
            pid.SetMode(MANUAL);
            pid.SetMode(AUTOMATIC);

            state = SOAK;
            stateStart = millis();
        }
        break;
    }

    case SOAK:
        setpoint = p.soakTemp;
        if (elapsed > p.soakTime * 1000)
        {

            pid.SetMode(MANUAL);
            pid.SetMode(AUTOMATIC);

            state = REFLOW;
            stateStart = millis();
        }
        break;

    case REFLOW:
        setpoint = p.reflowTemp;
        if (elapsed > p.reflowTime * 1000)
        {
            pid.SetMode(MANUAL);
            state = COOLDOWN;
        }
        break;

    case COOLDOWN:
        pid.SetMode(MANUAL);
        digitalWrite(SSR_PIN, HIGH);
        if (temperature < 50)
        {
            pid.SetMode(AUTOMATIC);

            state = IDLE;
        }
        break;
    }
}