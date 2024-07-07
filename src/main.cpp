#include "PressureMonitor.h"

#define BUZZER 7
#define SENSOR_PIN A0

RCSwitch rcSwitch = RCSwitch();

PressureMonitor pressureMonitor;

// Definition der Schwellenwerte
ThresholdValues thresholdValues[] = {
    {1631, 1351, 1131, 0},
    {1531, 1271, 1071, 0},
    {163, 135, 113, 0}};

// Funktion zur Auswahl der ThresholdValues
void initializePressureMonitor(PressureMonitor &pressureMonitor)
{
  pressureMonitor.displayInit();
  delay(2000);
  int resultat = pressureMonitor.readResult();
  for (size_t i = 0; i < sizeof(thresholdValues) / sizeof(thresholdValues[0]); ++i)
  {
    if (resultat >= (thresholdValues[i].knickLow - 100) && resultat <= (thresholdValues[i].knickLow + 100))
    {
      pressureMonitor = PressureMonitor(thresholdValues[i]);
      break;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  rcSwitch.enableTransmit(10);
  // Initialisiere LED-Pins und Buzzer
  int ledPins[] = {LED_GRUEN, LED_GELB, LED_ROT, BUZZER};
  for (int i = 0; i < 4; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  pinMode(PIN_OUT, INPUT);
  pinMode(PIN_SCK, OUTPUT);
  // Initialisiere den PressureMonitor mit den passenden ThresholdValues
  pressureMonitor = PressureMonitor(thresholdValues[0]);
  // initializePressureMonitor(pressureMonitor);
}

void loop()
{
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED_GRUEN, LOW);
  digitalWrite(LED_GELB, LOW);
  digitalWrite(LED_ROT, LOW);

  // warte auf den aktuellen Messwert
  while (digitalRead(PIN_OUT))
  {
  }

  // Pulse die Clock Line 3 mal, um die nächste Druckmessung zu starten
  for (byte i = 0; i < 3; i++)
  {
    digitalWrite(PIN_SCK, HIGH);
    digitalWrite(PIN_SCK, LOW);
  }

  int resultat = pressureMonitor.readResult();

  // Konvertiere 'resultat' zu 'const char*'
  char sendValue[10];
  itoa(resultat, sendValue, 10); // Konvertiert 'resultat' in eine Zeichenkette

  // Überprüfe Schwellenwerte
  Threshold *matchingThreshold = pressureMonitor.getMatchingThreshold(resultat);
  if (matchingThreshold != nullptr)
  {
    Serial.println(resultat);
    digitalWrite(matchingThreshold->ledPin, HIGH);
    if (matchingThreshold->buzzer)
    {
      digitalWrite(BUZZER, HIGH);
    }
    rcSwitch.send(sendValue);
  }
  pressureMonitor.displayResult();
  delay(500);
}
