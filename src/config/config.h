#pragma once
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PID_v1.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Wire.h>

// ==== WiFi ====
extern const char *ssid;
extern const char *password;

// ==== Server ====
extern AsyncWebServer server;
extern AsyncWebSocket ws;

// ==== Pins ====
#define SSR_PIN 26
#define THERM_PIN 34
#define OLED_SDA 21
#define OLED_SCL 22

// ==== OLED ====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
extern Adafruit_SSD1306 display;

// ==== Thermistor ====
#define SERIES_RESISTOR 10000
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMP 25
#define BETA_COEFFICIENT 3950

// ==== PID ====
extern double temperature, setpoint, pidOutput;
extern double kp, ki, kd;
extern PID pid;

// ==== ReflowState ====
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
extern ReflowState state;

// ==== Profiles ====
#define MAX_PROFILES 5
struct ReflowProfile
{
    char name[32];
    float preheatTemp;
    float soakTemp;
    float reflowTemp;
    int preheatTime;
    int soakTime;
    int reflowTime;
};

extern ReflowProfile profiles[MAX_PROFILES];
extern int profileCount;
extern int activeProfile;