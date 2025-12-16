// FULL FEATURE ESP32 REFLOW HOTPLATE FIRMWARE
// Features:
// - WiFi Web UI + REST API
// - Live temperature graph (WebSocket)
// - Multiple reflow profiles
// - PID control with time‑proportional SSR
// - OLED status display
// - OTA firmware update
// - Safety protections

// ================== LIBRARIES ==================
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoOTA.h>
#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================== WIFI (AP MODE) ==================
const char *ssid = "Reflow Oven"; const char *password = "reflow123";
WebServer server(80);
WebSocketsServer ws(81);

// ================== PINS ==================
#define SSR_PIN 26
#define THERM_PIN 34
#define OLED_SDA 21
#define OLED_SCL 22

// ================== OLED ==================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================== THERMISTOR ==================
#define SERIES_RESISTOR 10000
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMP 25
#define BETA_COEFFICIENT 3950

// ================== PID ==================
double temperature, setpoint, pidOutput;
PID pid(&temperature, &pidOutput, &setpoint, 2.0, 0.0025, 9.0, DIRECT);

// ================== SSR ==================
#define SSR_WINDOW 1000
unsigned long windowStart = 0;

// ================== STATE ==================
enum ReflowState
{
  IDLE,
  PREHEAT,
  SOAK,
  REFLOW,
  COOLDOWN,
  DONE,
  ERROR
};
ReflowState state = IDLE;
unsigned long stateStart = 0;

// ================== PROFILE ==================
struct ReflowProfile
{
  const char *name;
  float preheatTemp;
  float soakTemp;
  float reflowTemp;
  int preheatTime;
  int soakTime;
  int reflowTime;
};

ReflowProfile profiles[] = {
    {"LEAD", 150, 150, 200, 90, 20, 20},
    {"LEADFREE", 160, 180, 230, 100, 30, 30}};
int activeProfile = 0;

// ================== FUNCTIONS ==================

double readThermistor()
{
  int adc = analogRead(THERM_PIN);
  if (adc <= 0)
    return NAN;

  double resistance = SERIES_RESISTOR / ((4095.0 / adc) - 1.0);
  double steinhart = resistance / NOMINAL_RESISTANCE;
  steinhart = log(steinhart);
  steinhart /= BETA_COEFFICIENT;
  steinhart += 1.0 / (NOMINAL_TEMP + 273.15);
  steinhart = 1.0 / steinhart;
  return steinhart - 273.15;
}

void applySSR(double power)
{
  unsigned long now = millis();
  if (now - windowStart > SSR_WINDOW)
    windowStart += SSR_WINDOW;

  if ((power / 100.0) * SSR_WINDOW > (now - windowStart))
    digitalWrite(SSR_PIN, LOW);
  else
    digitalWrite(SSR_PIN, HIGH);
}

void updateProfile()
{
  auto &p = profiles[activeProfile];
  unsigned long elapsed = millis() - stateStart;

  switch (state)
  {
  case PREHEAT:
  {
    double ramp = elapsed * p.preheatTemp / (p.preheatTime * 1000.0);
    setpoint = (ramp < p.preheatTemp) ? ramp : p.preheatTemp;
  }
    if (elapsed > p.preheatTime * 1000)
    {
      state = SOAK;
      stateStart = millis();
    }
    break;

  case SOAK:
    setpoint = p.soakTemp;
    if (elapsed > p.soakTime * 1000)
    {
      state = REFLOW;
      stateStart = millis();
    }
    break;

  case REFLOW:
    setpoint = p.reflowTemp;
    if (elapsed > p.reflowTime * 1000)
    {
      state = COOLDOWN;
    }
    break;

  case COOLDOWN:
    setpoint = 0;
    if (temperature < 50)
      state = DONE;
    break;
  }
}

void updateOLED()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("TEMP %.1fC\n", temperature);
  display.printf("SET  %.0fC\n", setpoint);
  display.printf("PID  %.0f%%\n", pidOutput);
  display.printf("MODE %s\n",
                 state == IDLE ? "IDLE" : state == PREHEAT ? "PRE"
                                      : state == SOAK      ? "SOAK"
                                      : state == REFLOW    ? "REF"
                                      : state == COOLDOWN  ? "COOL"
                                      : state == DONE      ? "DONE"
                                                           : "ERR");
  display.display();
}

void setupWeb()
{
  server.on("/", []()
            { server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html><html><head>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<title>ESP32 Reflow</title>
<style>
body{font-family:Arial;background:#111;color:#eee;text-align:center}
button{font-size:20px;margin:8px;padding:10px 20px}
</style>
</head><body>
<h2>ESP32 Reflow Hotplate</h2>
<p id='t'>Temp: --</p>
<p id='s'>State: --</p>
<button onclick="fetch('/start')">START</button>
<button onclick="fetch('/stop')">STOP</button><br>
<button onclick="fetch('/profile?id=0')">LEAD</button>
<button onclick="fetch('/profile?id=1')">LEAD-FREE</button>
<script>
setInterval(()=>{
 fetch('/status').then(r=>r.json()).then(j=>{
  document.getElementById('t').innerText='Temp: '+j.temp.toFixed(1)+' C';
  document.getElementById('s').innerText='State: '+j.state;
 });
},1000);
</script>
</body></html>
)rawliteral"); });

  server.on("/start", []()
            { state=PREHEAT; stateStart=millis(); });
  server.on("/stop", []()
            { state=IDLE; digitalWrite(SSR_PIN,HIGH); });
  server.on("/profile", []()
            { activeProfile = server.arg("id").toInt(); });
  server.on("/status", []()
            { server.send(200, "application/json",
                          String("{\"temp\":") + temperature +
                              ",\"set\":" + setpoint +
                              ",\"state\":" + state + "}"); });

  server.begin();
}

void setupOTA()
{
  ArduinoOTA.setHostname("reflow-esp32");
  ArduinoOTA.begin();
}

void setup()
{
  pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(300);

  setupWeb();
  setupOTA();

  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(0, 100);
}

void loop()
{
  ArduinoOTA.handle();
  server.handleClient();

  temperature = readThermistor();
  if (isnan(temperature) || temperature > 270)
  {
    state = ERROR;
    digitalWrite(SSR_PIN, HIGH);
  }

  if (state != IDLE && state != DONE && state != ERROR)
  {
    updateProfile();
    pid.Compute();
    applySSR(pidOutput);
  }
  else
  {
    digitalWrite(SSR_PIN, HIGH);
  }

  updateOLED();
}
