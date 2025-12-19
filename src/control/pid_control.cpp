#include "config/config.h"
#include <PID_v1.h>

void savePID()
{
    JsonDocument doc;
    doc["kp"] = kp;
    doc["ki"] = ki;
    doc["kd"] = kd;

    File f = LittleFS.open("/pid.json", "w");
    if (!f)
        return;

    serializeJson(doc, f);
    f.close();
}

void loadPID()
{
    if (!LittleFS.exists("/pid.json"))
    {
        // Defaults
        kp = 2.0;
        ki = 0.0025;
        kd = 9.0;

        pid.SetTunings(kp, ki, kd);
        savePID();
        return;
    }

    File f = LittleFS.open("/pid.json", "r");
    if (!f)
        return;

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, f);
    f.close();

    if (err)
    {
        Serial.println("Failed to parse pid.json");
        return;
    }

    kp = doc["kp"] | kp;
    ki = doc["ki"] | ki;
    kd = doc["kd"] | kd;

    pid.SetTunings(kp, ki, kd);
}

void updatePID(double newKp, double newKi, double newKd)
{
    kp = newKp;
    ki = newKi;
    kd = newKd;

    pid.SetTunings(kp, ki, kd);
    savePID(); // optional: persist immediately
}
