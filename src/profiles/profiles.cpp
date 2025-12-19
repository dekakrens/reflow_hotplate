#include "config/config.h"

void saveProfiles()
{
    JsonDocument doc;
    JsonArray profileArray = doc["profiles"].to<JsonArray>();

    for (int i = 0; i < profileCount; i++)
    {
        JsonObject o = profileArray.add<JsonObject>();
        o["name"] = profiles[i].name;
        o["preheatTemp"] = profiles[i].preheatTemp;
        o["soakTemp"] = profiles[i].soakTemp;
        o["reflowTemp"] = profiles[i].reflowTemp;
        o["preheatTime"] = profiles[i].preheatTime;
        o["soakTime"] = profiles[i].soakTime;
        o["reflowTime"] = profiles[i].reflowTime;
    }

    File f = LittleFS.open("/profiles.json", "w");
    serializeJson(doc, f);
    f.close();
}

void loadProfiles()
{
    if (!LittleFS.exists("/profiles.json"))
    {
        strlcpy(profiles[0].name, "LEAD", sizeof(profiles[0].name));
        profiles[0].preheatTemp = 150;
        profiles[0].soakTemp = 150;
        profiles[0].reflowTemp = 200;
        profiles[0].preheatTime = 90;
        profiles[0].soakTime = 20;
        profiles[0].reflowTime = 20;

        strlcpy(profiles[1].name, "LEADFREE", sizeof(profiles[1].name));
        profiles[1].preheatTemp = 160;
        profiles[1].soakTemp = 180;
        profiles[1].reflowTemp = 230;
        profiles[1].preheatTime = 100;
        profiles[1].soakTime = 30;
        profiles[1].reflowTime = 30;

        profileCount = 2;

        saveProfiles();
        return;
    }

    File f = LittleFS.open("/profiles.json", "r");
    if (!f)
        return;

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, f);
    f.close();

    if (err)
    {
        Serial.println("Failed to parse profiles.json");
        return;
    }

    JsonArray arr = doc["profiles"].as<JsonArray>();
    profileCount = 0;

    for (JsonObject o : arr)
    {
        if (profileCount >= MAX_PROFILES)
            break;

        strlcpy(
            profiles[profileCount].name,
            o["name"] | "",
            sizeof(profiles[profileCount].name));
        profiles[profileCount].preheatTemp = o["preheatTemp"] | 0;
        profiles[profileCount].soakTemp = o["soakTemp"] | 0;
        profiles[profileCount].reflowTemp = o["reflowTemp"] | 0;
        profiles[profileCount].preheatTime = o["preheatTime"] | 0;
        profiles[profileCount].soakTime = o["soakTime"] | 0;
        profiles[profileCount].reflowTime = o["reflowTime"] | 0;

        profileCount++;
    }
}
