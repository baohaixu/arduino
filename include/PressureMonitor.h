#ifndef PRESSUREMONITOR_H
#define PRESSUREMONITOR_H

#include "Threshold.h"
#define LED_GRUEN 4
#define LED_GELB 5
#define LED_ROT 6

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

  PressureMonitor(const ThresholdValues &values);
  Threshold *getMatchingThreshold(int value);
};

#endif // PRESSUREMONITOR_H
