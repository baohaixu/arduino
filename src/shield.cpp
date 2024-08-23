#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Touchscreen settings
#define YP A1  // Must be an analog pin, use "An" notation!
#define XM A2  // Must be an analog pin, use "An" notation!
#define YM 7   // Can be a digital pin
#define XP 6   // Can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Define button boundaries
#define BUTTON_X 60
#define BUTTON_Y 100
#define BUTTON_W 100
#define BUTTON_H 50

// Define pages
enum Page { PAGE_HOME, PAGE_SETTINGS };
Page currentPage = PAGE_HOME;

void setup() {
  Serial.begin(9600);
  tft.begin(0x9341);  // Initialize with your display's driver code
  tft.setRotation(1);

  showPage(currentPage);
}

void loop() {
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  // If the screen is touched
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    int x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    int y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    switch (currentPage) {
      case PAGE_HOME:
        if (x >= BUTTON_X && x <= BUTTON_X + BUTTON_W && y >= BUTTON_Y && y <= BUTTON_Y + BUTTON_H) {
          Serial.println("Go to Settings!");
          currentPage = PAGE_SETTINGS;
          showPage(currentPage);
        }
        break;
      
      case PAGE_SETTINGS:
        if (x >= BUTTON_X && x <= BUTTON_X + BUTTON_W && y >= BUTTON_Y && y <= BUTTON_Y + BUTTON_H) {
          Serial.println("Back to Home!");
          currentPage = PAGE_HOME;
          showPage(currentPage);
        }
        break;
    }
  }
}

// Function to draw a button
void drawButton(const char* label, int x, int y, int w, int h, uint16_t textColor, uint16_t bgColor) {
  tft.fillRect(x, y, w, h, bgColor);
  tft.drawRect(x, y, w, h, textColor);
  tft.setCursor(x + (w/2) - (strlen(label) * 3), y + (h/2) - 4);
  tft.setTextColor(textColor);
  tft.setTextSize(2);
  tft.print(label);
}

// Function to display the current page
void showPage(Page page) {
  tft.fillScreen(BLACK);

  switch (page) {
    case PAGE_HOME:
      tft.setCursor(50, 50);
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.print("Home Page");
      drawButton("Settings", BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, WHITE, BLUE);
      break;
      
    case PAGE_SETTINGS:
      tft.setCursor(50, 50);
      tft.setTextColor(WHITE);
      tft.setTextSize(2);
      tft.print("Settings Page");
      drawButton("Back", BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, WHITE, BLUE);
      break;
  }
}