#include "config/config.h"

#define SSR_WINDOW 1000
unsigned long windowStart = 0;
unsigned long stateStart = 0;

void applySSR(double power)
{
    unsigned long now = millis();
    if (now - windowStart >= SSR_WINDOW)
        windowStart = now;

    if ((power / 100.0) * SSR_WINDOW > (now - windowStart))
        digitalWrite(SSR_PIN, LOW);
    else
        digitalWrite(SSR_PIN, HIGH);
}

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

            state = DONE;
        }
        break;
    }
}