#ifndef PRESSUREMONITOR_H
#define PRESSUREMONITOR_H
#include <RCSwitch.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Threshold.h"
#define LED_GRUEN 4
#define LED_GELB 5
#define LED_ROT 6
#define PIN_OUT 2
#define PIN_SCK 3
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

struct ThresholdValues
{
  int knickLow;
  int verdrehtLow;
  int normalLow;
  int entferntLow;
};

class PressureMonitor
{
public:
  Threshold knick;
  Threshold verdreht;
  Threshold normal;
  Threshold entfernt;
  PressureMonitor();
  PressureMonitor(const ThresholdValues &values);
  Threshold *getMatchingThreshold(int value);
  int readResult();
  void displayResult();
  void displayInit();

private:
  Threshold *status = nullptr;
  int resultat;
  Adafruit_SSD1306 display;
};

#endif // PRESSUREMONITOR_H
