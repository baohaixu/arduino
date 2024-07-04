#include "PressureMonitor.h"

#define INT_MAX 2147483647L

PressureMonitor::PressureMonitor(const ThresholdValues &values)
    : knick(values.knickLow, INT_MAX, "K N I C K", LED_ROT, true),
      verdreht(values.verdrehtLow, values.knickLow - 1, " VERDREHT", LED_ROT, true),
      normal(values.normalLow, values.verdrehtLow - 1, " NORMAL", LED_GRUEN, false),
      entfernt(values.entferntLow, values.normalLow - 1, " entfernt", LED_GELB, true) {}

Threshold *PressureMonitor::getMatchingThreshold(int value)
{
  if (value >= knick.low && value <= knick.high)
    return &knick;
  if (value >= verdreht.low && value <= verdreht.high)
    return &verdreht;
  if (value >= normal.low && value <= normal.high)
    return &normal;
  if (value >= entfernt.low && value <= entfernt.high)
    return &entfernt;
  return nullptr;
}
