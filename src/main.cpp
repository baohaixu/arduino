#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RCSwitch.h>
#include "PressureMonitor.h"

#define PIN_OUT 2
#define PIN_SCK 3
#define BUZZER 7
#define SENSOR_PIN A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

RCSwitch rcSwitch = RCSwitch();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define the threshold values
ThresholdValues thresholdValues[] = {
    {1631, 1351, 1131, 0},
    {163, 135, 113, 0}};

// Initialize PressureMonitor instances
PressureMonitor pressureMonitor1(thresholdValues[0]);
PressureMonitor pressureMonitor2(thresholdValues[1]);

PressureMonitor &pressureMonitor = pressureMonitor1; // Select which one to use

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

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // eventuell 0x3D - benutze den I2C_Scanner
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

  // Pulse die Clock Line 3 mal, um die nächste Druckmessung zu starten
  for (byte i = 0; i < 3; i++)
  {
    digitalWrite(PIN_SCK, HIGH);
    digitalWrite(PIN_SCK, LOW);
  }

  Serial.println(result);
  int resultat = result / 10000;

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

  // Konvertiere 'resultat' zu 'const char*'
  char sendValue[10];
  itoa(resultat, sendValue, 10); // Konvertiert 'resultat' in eine Zeichenkette

  // Überprüfe Schwellenwerte
  Threshold *matchingThreshold = pressureMonitor.getMatchingThreshold(resultat);
  if (matchingThreshold != nullptr)
  {
    Serial.println(resultat);
    display.println(matchingThreshold->message);
    digitalWrite(matchingThreshold->ledPin, HIGH);
    if (matchingThreshold->buzzer)
    {
      digitalWrite(BUZZER, HIGH);
    }
    rcSwitch.send(sendValue);
  }

  display.display();
  delay(500);
}
