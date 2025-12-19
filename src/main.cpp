// ================== LIBRARIES ==================
#include <Arduino.h>
#include "config/config.h"
#include "display/oled.h"
#include "control/pid_control.h"
#include "profiles/profiles.h"
#include "reflow/reflow.h"
#include "sensors/thermistor.h"
#include "web/web_server.h"

// ===== WIFI (AP MODE) =====
const char *ssid = "Reflow Hotplate";
const char *password = "reflow123";

// ===== OLED =====
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== Server =====
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// ===== Mode =====
MachineMode mode = MODE_IDLE;

// ===== Reflow =====
ReflowState state = IDLE;
ReflowProfile profiles[MAX_PROFILES];
int profileCount = 0;
int activeProfile = 0;

// ===== PID =====
double temperature, setpoint, pidOutput;
double kp = 2.0, ki = 0.0025, kd = 9.0;
PID pid(&temperature, &pidOutput, &setpoint, kp, ki, kd, DIRECT);

// ===== SSR =====
#define SSR_WINDOW 1000
unsigned long windowStart = 0;

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

void setup()
{
  Serial.begin(115200);
  pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, HIGH);

  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(0, 100);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  loadPID();
  loadProfiles();

  setupWeb();
}

void loop()
{
  temperature = readThermistor();

  switch (mode)
  {
  case MODE_REFLOW:
  {
    if (state != IDLE && state != ERROR)
    {
      reflowStateChange();
      pid.Compute();
      applySSR(pidOutput);
    }
    else
    {
      digitalWrite(SSR_PIN, HIGH);
    }
    break;
  }

  case MODE_HEATER:
  {
    pid.Compute();
    applySSR(pidOutput);
    break;
  }

  default:
  {
    digitalWrite(SSR_PIN, HIGH);
    pid.SetMode(MANUAL);
    break;
  }
  }

  updateOLED();
  notifyClient();
}
