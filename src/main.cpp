#define PIN_OUT 2
#define PIN_SCK 3
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#include <RCSwitch.h>

RCSwitch rcSwitch = RCSwitch();

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Drucksensor

int Sensorpin = A0; // Definition, an welcher Stelle am Mikrocontroller der Sensorwert ausgelesen werden soll

// int a;

void setup()
{

  Serial.begin(9600);
  // Sendemodul an dem digitalen PIN 10 angeschlossen.
  rcSwitch.enableTransmit(10);

  pinMode(7, OUTPUT); // setze digitalpin 7 auf output

  pinMode(4, OUTPUT); // LED Grün
  pinMode(5, OUTPUT); // LED GELB
  pinMode(7, OUTPUT); // LED ROT

  pinMode(PIN_OUT, INPUT);
  pinMode(PIN_SCK, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // eventuell 0x3D - benutze den I2C_Scanner

  Serial.begin(9600);
}

void loop()
{
  digitalWrite(7, LOW); // buzzer++

  digitalWrite(4, LOW); // LED orange
  digitalWrite(5, LOW); // lED ROT
  digitalWrite(6, LOW); // LED grün

  // wait for the current reading to finish
  while (digitalRead(PIN_OUT))
  {
  }

  // read 24 bits
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

  // get the 2s compliment
  result = result ^ 0x800000;

  // pulse the clock line 3 times to start the next pressure reading
  for (byte i = 0; i < 3; i++)
  {
    digitalWrite(PIN_SCK, HIGH);
    digitalWrite(PIN_SCK, LOW);
  }

  Serial.println(result);
  int resultat;
  resultat = result / 10000;

  ////   mittleren Wert ermitteln///////////////////////////

  // resultat = map(resultat, 780, 1800, 780, 1800);

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

  if (resultat > 1630)
  {

    Serial.println(resultat);
    display.println("K N I C K");
    digitalWrite(7, HIGH);
    digitalWrite(5, HIGH);

    // Senden von resultat
    rcSwitch.send(sendValue);
  }

  if ((resultat > 1350) and resultat < 1500)
  {

    Serial.println(resultat);

    display.println(" VERDREHT");
    digitalWrite(7, HIGH);
    digitalWrite(5, HIGH);

    // Senden von resultat
    rcSwitch.send(sendValue);
  }

  if ((resultat > 1130) and resultat < 1350)
  {

    Serial.println(resultat);

    display.println(" NORMAL");
    digitalWrite(4, HIGH);

    // Senden von resultat
    rcSwitch.send(sendValue);
  }

  if (resultat < 1150)
  {

    Serial.println(resultat);
    Serial.println("entfernt");
    display.println(" entfernt");
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);

    // Senden von resultat
    rcSwitch.send(sendValue);

    // if (resultat=758);
    // Serial.print(resultat);
    // if (resultat=0);
    // Serial.print(resultat);

    // delay(1000);
  }

  display.display();
  delay(500);
}

// Arduino Anschluss
// OLED GND – Arduino GND
//  OLED VCC – Arduino 5V
// OLED SCL – Arduino A5
//  OLED SDA – Arduino A4