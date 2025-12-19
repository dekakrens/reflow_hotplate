#include "config/config.h"
#include "profiles/profiles.h"
#include "control/pid_control.h"

unsigned long lastNotify = 0;

void notifyClient()
{
  unsigned long now = millis();

  if (now - lastNotify < 500)
    return; // too soon, skip

  lastNotify = now;

  JsonDocument doc;
  doc["temp"] = temperature;
  doc["set"] = setpoint;
  doc["state"] = state;

  String payload;
  serializeJson(doc, payload);
  ws.textAll(payload);
}

void setupWeb()
{
  server.serveStatic("/", LittleFS, "/")
      .setDefaultFile("index.html")
      .setCacheControl("max-age=31536000");

  server.on("/api/profiles", HTTP_GET, [](AsyncWebServerRequest *req)
            {
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    JsonArray arr = root["profiles"].to<JsonArray>();

    for (int i = 0; i<profileCount; i++) {
      auto &p = profiles[i];
      JsonObject o = arr.createNestedObject();
      o["name"] = p.name;
      o["preheatTemp"] = p.preheatTemp;
      o["soakTemp"] = p.soakTemp;
      o["reflowTemp"] = p.reflowTemp;
      o["preheatTime"] = p.preheatTime;
      o["soakTime"] = p.soakTime;
      o["reflowTime"] = p.reflowTime;
    }

    String responseBody;
    serializeJson(doc, responseBody);
    req->send(200, "application/json", responseBody); });

  // server.on(
  //     "/api/profiles",
  //     HTTP_POST,
  //     [](AsyncWebServerRequest *req) {},
  //     nullptr,
  //     [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t, size_t)
  //     {
  //       if (profileCount >= MAX_PROFILES)
  //       {
  //         req->send(409, "text/plain", "Profile limit reached");
  //         return;
  //       }

  //       JsonDocument doc;
  //       DeserializationError err = deserializeJson(doc, data, len);
  //       if (err)
  //       {
  //         req->send(400, "text/plain", "Invalid JSON");
  //         return;
  //       }

  //       // Validate required fields
  //       if (!doc.containsKey("name"))
  //       {
  //         req->send(400, "text/plain", "Missing name");
  //         return;
  //       }

  //       // Create new profile safely
  //       ReflowProfile &p = profiles[profileCount];

  //       strlcpy(
  //           p.name,
  //           doc["name"] | "",
  //           sizeof(p.name));

  //       p.preheatTemp = doc["preheatTemp"] | 0;
  //       p.soakTemp = doc["soakTemp"] | 0;
  //       p.reflowTemp = doc["reflowTemp"] | 0;
  //       p.preheatTime = doc["preheatTime"] | 0;
  //       p.soakTime = doc["soakTime"] | 0;
  //       p.reflowTime = doc["reflowTime"] | 0;

  //       profileCount++;

  //       saveProfiles();

  //       req->send(201, "application/json", "{\"status\":\"ok\"}");
  //     });

  server.on("/api/pid", HTTP_PUT, [](AsyncWebServerRequest *req) {}, nullptr, [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t, size_t)
            {
      JsonDocument doc;
      JsonDocument responseDoc;
      String responseBody;

      if (deserializeJson(doc, data)) {
        responseDoc["status"] = "failed";
        responseDoc["message"] = "Unable to deserialize the payload";
        serializeJson(responseDoc, responseBody);
        req->send(400, "application/json", responseBody);
        return;
      }

      kp = doc["kp"];
      ki = doc["ki"];
      kd = doc["kd"];

      updatePID(kp, ki, kd);

      responseDoc["status"] = "success";
      responseDoc["message"] = "PID updated successfully";
      serializeJson(responseDoc, responseBody);
      req->send(200, "application/json", responseBody); });

  server.on("/api/heater-start", HTTP_POST, [](AsyncWebServerRequest *req) {}, nullptr, [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t, size_t)
            {
      JsonDocument responseDoc;
      String responseBody;

      JsonDocument doc;
      deserializeJson(doc, data);
      setpoint = doc["temperature"];

      responseDoc["status"] = "success";
      responseDoc["message"] = "Heater started successfully";
      responseDoc["temperature"] = setpoint;
      serializeJson(responseDoc, responseBody);
      req->send(200, "application/json", responseBody); });

  server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *req)
            {
  JsonDocument doc;

  // ----- PID -----
  JsonObject pidObj = doc.createNestedObject("pid");
  pidObj["kp"] = kp;
  pidObj["ki"] = ki;
  pidObj["kd"] = kd;

  // ----- PROFILES -----
  JsonArray arr = doc.createNestedArray("profiles");

  for (int i = 0; i < profileCount; i++)
  {
    auto &p = profiles[i];
    JsonObject o = arr.createNestedObject();

    o["name"] = p.name;
    o["preheatTemp"] = p.preheatTemp;
    o["soakTemp"] = p.soakTemp;
    o["reflowTemp"] = p.reflowTemp;
    o["preheatTime"] = p.preheatTime;
    o["soakTime"] = p.soakTime;
    o["reflowTime"] = p.reflowTime;
  }

  String json;
  serializeJson(doc, json);
  req->send(200, "application/json", json); });

  server.onNotFound([](AsyncWebServerRequest *req)
                    {
    if(req->method() == HTTP_GET) {
      req->send(LittleFS, "/index.html", "text/html");
    } });

  /* ---------- WEBSOCKET ---------- */
  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client,
                AwsEventType type, void *, uint8_t *, size_t)
             {

    if (type == WS_EVT_CONNECT) {
      notifyClient();
    } });

  server.addHandler(&ws);
  server.begin();
}