#define PIN_OUT 2
#define PIN_SCK 3
#define LED_GRUEN 4
#define LED_GELB 5
#define LED_ROT 6
#define BUZZER 7
#define SENSOR_PIN A0

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RCSwitch.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

RCSwitch rcSwitch = RCSwitch();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Struktur für Schwellenwerte
struct Thresholds
{
  int low;
  long high;
  const char *message;
  int ledPin;
  bool buzzer;
};

// Größter möglicher Wert für einen int
const long INT_MAX = 2147483647L;

// Initialisiere Schwellenwerte
Thresholds thresholds[] = {
    {1631, INT_MAX, "K N I C K", LED_ROT, true},
    {1351, 1500, " VERDREHT", LED_ROT, true},
    {1131, 1350, " NORMAL", LED_GRUEN, false},
    {0, 1150, " entfernt", LED_GELB, true}};

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
  for (size_t i = 0; i < sizeof(thresholds) / sizeof(thresholds[0]); i++)
  {
    if (resultat >= thresholds[i].low && resultat <= thresholds[i].high)
    {
      Serial.println(resultat);
      display.println(thresholds[i].message);
      digitalWrite(thresholds[i].ledPin, HIGH);
      if (thresholds[i].buzzer)
      {
        digitalWrite(BUZZER, HIGH);
      }
      rcSwitch.send(sendValue);
      break;
    }
  }

  display.display();
  delay(500);
}
