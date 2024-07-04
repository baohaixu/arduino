#include "PressureMonitor.h"

#define INT_MAX 2147483647L
PressureMonitor::PressureMonitor()
    : knick(0, INT_MAX, "K N I C K", LED_ROT, true),
      verdreht(0, 0, " VERDREHT", LED_ROT, true),
      normal(0, 0, " NORMAL", LED_GRUEN, false),
      entfernt(0, 0, " entfernt", LED_GELB, true),
      display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
{
}
PressureMonitor::PressureMonitor(const ThresholdValues &values)
    : knick(values.knickLow, INT_MAX, "K N I C K", LED_ROT, true),
      verdreht(values.verdrehtLow, values.knickLow - 1, " VERDREHT", LED_ROT, true),
      normal(values.normalLow, values.verdrehtLow - 1, " NORMAL", LED_GRUEN, false),
      entfernt(values.entferntLow, values.normalLow - 1, " entfernt", LED_GELB, true),
      display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(12, 0);
  display.println("Wert:");
  display.setCursor(72, 0);
  display.println(resultat);
  display.setTextSize(2);
  display.setCursor(0, 26);
  display.println(" SCHLAUCH");
  display.setTextSize(2);
  display.setCursor(0, 50);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // eventuell 0x3D - benutze den I2C_Scanner
}

Threshold *PressureMonitor::getMatchingThreshold(int value)
{
  if (value >= knick.low && value <= knick.high)
    status = &knick;
  if (value >= verdreht.low && value <= verdreht.high)
    status = &verdreht;
  if (value >= normal.low && value <= normal.high)
    status = &normal;
  if (value >= entfernt.low && value <= entfernt.high)
    status = &entfernt;

  return status;
}

int PressureMonitor::readResult()
{
  // Lese 24 Bits
  long result = 0;
  for (int i = 0; i < 24; i++)
  {
    digitalWrite(PIN_SCK, HIGH);
    digitalWrite(PIN_SCK, LOW);
    result = result << 1;
    if (digitalRead(PIN_OUT))
    {
      result++;
    }
  }

  // Konvertiere zu 2er-Komplement
  result = result ^ 0x800000;
  resultat = result / 10000;
  Serial.println(result);
  return resultat;
}

void PressureMonitor::displayResult()
{
  display.println(status->message);
  display.display();
}

void PressureMonitor::displayInit()
{
  display.println("Bitte knicken");
  display.display();
}