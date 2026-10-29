
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_SSD1306.h>
#include <TJpg_Decoder.h>

#include "frames.h"

// ===================================
// LARGE TFT - VIDEO SCREEN
// ===================================

#define TFT_BL   9
#define TFT_CS   10
#define TFT_DC   11
#define TFT_RST  12
#define TFT_MOSI 13
#define TFT_SCLK 14

// ===================================
// SMALL OLED - ID SCREEN
// ===================================

#define OLED_SCL 16
#define OLED_SDA 17

// ===================================
// DISPLAYS
// ===================================

Adafruit_ST7735 tft(
  &SPI, TFT_CS, TFT_DC, TFT_RST
);

Adafruit_SSD1306 oled(
  128, 64, &Wire, -1
);

bool oledReady = false;

// ===================================
// JPEG FRAME CALLBACK
// ===================================

bool drawJpgBlock(
  int16_t x,
  int16_t y,
  uint16_t w,
  uint16_t h,
  uint16_t *pixels
) {
  if (x >= 128 || y >= 160) {
    return false;
  }

  tft.drawRGBBitmap(
    x, y, pixels, w, h
  );

  return true;
}

// ===================================
// ORIGINAL CAT ICON
// ===================================

void drawCatIcon(int x, int y) {

  oled.drawLine(x+3,y+10,x+1,y+1,SSD1306_WHITE);
  oled.drawLine(x+1,y+1,x+11,y+6,SSD1306_WHITE);
  oled.drawLine(x+11,y+6,x+19,y+6,SSD1306_WHITE);
  oled.drawLine(x+19,y+6,x+29,y+1,SSD1306_WHITE);
  oled.drawLine(x+29,y+1,x+27,y+10,SSD1306_WHITE);

  oled.drawRoundRect(
    x+2,y+7,26,20,5,SSD1306_WHITE
  );

  // Blindfold
  oled.fillRoundRect(
    x+5,y+13,20,6,2,SSD1306_WHITE
  );

  // X mouth
  oled.drawLine(
    x+12,y+21,x+18,y+25,SSD1306_WHITE
  );

  oled.drawLine(
    x+18,y+21,x+12,y+25,SSD1306_WHITE
  );
}

// ===================================
// SMALL OLED - ORIGINAL TEXT LAYOUT
// ===================================

void drawId() {

  if (!oledReady) return;

  oled.clearDisplay();

  // Reset to original built-in font
  oled.setFont();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextWrap(false);

  // Battery - top right
  oled.setCursor(77, 0);
  oled.print("BAT --%");

  // Original cat icon
  drawCatIcon(3, 17);

  // Full name - original readable font
  oled.setFont();
  oled.setTextSize(1);
  oled.setCursor(43, 29);
  oled.print("JAFET DUARTE");

  // Electrical Engineer - one line
  oled.setFont();
  oled.setTextSize(1);
  oled.setCursor(14, 55);
  oled.print("ELECTRICAL ENGINEER");

  oled.display();
}

// ===================================
// SETUP
// ===================================

void setup() {

  Serial.begin(115200);
  delay(250);

  // TFT initialization
  SPI.begin(
    TFT_SCLK,
    -1,
    TFT_MOSI,
    TFT_CS
  );

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);

  // Keep existing backlight wiring
  // GPIO 9 is not driven here

  // JPEG decoder
  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(drawJpgBlock);

  // OLED initialization
  Wire.begin(OLED_SDA, OLED_SCL);
  Wire.setClock(100000);

  oledReady = oled.begin(
    SSD1306_SWITCHCAPVCC,
    0x3C
  );

  if (oledReady) {
    drawId();
    Serial.println("OLED ready");
  } else {
    Serial.println("OLED not found");
  }

  Serial.println("Cyber ID ready");
}

// ===================================
// MAIN LOOP - VIDEO PLAYBACK
// ===================================

void loop() {

  static uint8_t frameIndex = 0;
  static uint32_t nextFrame = 0;

  // Target 10 FPS
  if ((int32_t)(millis() - nextFrame) >= 0) {

    uint32_t began = millis();

    // Draw video frame
    TJpgDec.drawJpg(
      0, 0,
      frames[frameIndex],
      frameSizes[frameIndex]
    );

    // Social handle footer
    tft.fillRect(
      0, 141,
      128, 19,
      ST77XX_BLACK
    );

    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextWrap(false);

    tft.setCursor(34, 147);
    tft.print("@Jafetdgc");

    // Loop video
    frameIndex =
      (frameIndex + 1) % FRAME_COUNT;

    nextFrame = began + 100;
  }
}
