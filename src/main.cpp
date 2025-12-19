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

// ===== Reflow =====
ReflowState state = IDLE;
ReflowProfile profiles[MAX_PROFILES];
int profileCount = 0;
int activeProfile = 0;

// ===== PID =====
double temperature, setpoint, pidOutput;
double kp = 2.0, ki = 0.0025, kd = 9.0;
PID pid(&temperature, &pidOutput, &setpoint, kp, ki, kd, DIRECT);

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
  if (state == ERROR)
  {
    digitalWrite(SSR_PIN, HIGH);
    pid.SetMode(MANUAL);
    return;
  }

  if (state != IDLE && state != DONE && state != ERROR)
  {
    reflowStateChange();
    pid.Compute();
    applySSR(pidOutput);
  }
  else
  {
    digitalWrite(SSR_PIN, HIGH);
  }

  updateOLED();
}
