#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_SSD1306.h>
#include <TJpg_Decoder.h>
#include "frames.h"

// Photo embedded in flash (source: firmware/assets/ID_two/pepper_source.jpg).
#include "pepper_photo.h"

// ESP32-S3 SuperMini, final agreed order. USB-C at top, labels facing you.
#define TFT_CS    1
#define TFT_DC    2
#define TFT_RST   3  // GPIO3 is an ESP32-S3 strapping pin; test reliable boot
#define TFT_MOSI  4
#define TFT_SCLK  5
#define BTN_MODE  6  // Momentary switch to GND
// GPIO7 deliberately unused; one button switches profiles.
#define OLED_SDA 12
#define OLED_SCL 13

#define TFT_W 128
#define TFT_H 160
#define OLED_W 128
#define OLED_H 64

Adafruit_ST7735 tft(&SPI, TFT_CS, TFT_DC, TFT_RST);
Adafruit_SSD1306 oled(OLED_W, OLED_H, &Wire, -1);
bool oledReady = false;

enum Profile : uint8_t { MY_ID = 0, PEPPER = 1 };
Profile currentProfile = MY_ID;
const uint16_t FRAME_INTERVAL_MS = 100; // ~10 FPS
uint16_t frameIndex = 0;
uint32_t nextFrameAt = 0;

// Debounce each switch separately; no blocking delays in loop.
struct Button {
  uint8_t pin;
  bool stable = HIGH;
  bool lastRead = HIGH;
  uint32_t changedAt = 0;
};
Button modeButton{BTN_MODE};

bool pressed(Button &b) {
  bool reading = digitalRead(b.pin);
  if (reading != b.lastRead) b.changedAt = millis();
  b.lastRead = reading;
  if ((uint32_t)(millis() - b.changedAt) >= 35 && reading != b.stable) {
    b.stable = reading;
    if (b.stable == LOW) return true;
  }
  return false;
}

bool drawJpgBlock(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *pixels) {
  if (x >= TFT_W || y >= TFT_H) return false;
  tft.drawRGBBitmap(x, y, pixels, w, h);
  return true;
}

void drawCatIcon(int x, int y) {
  oled.drawLine(x+3,y+10,x+1,y+1,SSD1306_WHITE);
  oled.drawLine(x+1,y+1,x+11,y+6,SSD1306_WHITE);
  oled.drawLine(x+11,y+6,x+19,y+6,SSD1306_WHITE);
  oled.drawLine(x+19,y+6,x+29,y+1,SSD1306_WHITE);
  oled.drawLine(x+29,y+1,x+27,y+10,SSD1306_WHITE);
  oled.drawRoundRect(x+2,y+7,26,20,5,SSD1306_WHITE);
  oled.fillRoundRect(x+5,y+13,20,6,2,SSD1306_WHITE);
  oled.drawLine(x+12,y+21,x+18,y+25,SSD1306_WHITE);
  oled.drawLine(x+18,y+21,x+12,y+25,SSD1306_WHITE);
}

void drawOLED() {
  if (!oledReady) return;
  oled.clearDisplay();
  oled.setFont();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextWrap(false);
  oled.setCursor(77, 0);
  oled.print("BAT --%"); // Placeholder, not a battery measurement
  drawCatIcon(3, 17);
  if (currentProfile == MY_ID) {
    oled.setCursor(43, 29);
    oled.print("JAFET DUARTE");
    oled.setCursor(14, 55);
    oled.print("ELECTRICAL ENGINEER");
  } else {
    oled.setTextSize(2);
    oled.setCursor(43, 24);
    oled.print("PEPPER");
    oled.setTextSize(1);
    oled.setCursor(39, 55);
    oled.print("CAT PROFILE");
  }
  oled.display();
}

void drawFooter(const char *text) {
  tft.fillRect(0, 141, TFT_W, 19, ST77XX_BLACK);
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(false);
  tft.setCursor(34, 147);
  tft.print(text);
}

void drawPepper() {
  tft.fillScreen(ST77XX_BLACK);
  // JPEG photo cropped to 128x141; footer occupies y=141..159.
  TJpgDec.drawJpg(0, 0, pepperPhoto, pepperPhotoSize);
  drawFooter("PEPPER");
}

void switchProfile() {
  currentProfile = (currentProfile == MY_ID) ? PEPPER : MY_ID;
  drawOLED();
  if (currentProfile == PEPPER) {
    drawPepper();
  } else {
    tft.fillScreen(ST77XX_BLACK);
    frameIndex = 0;
    nextFrameAt = 0;
  }
  Serial.printf("Profile: %s\n", currentProfile == MY_ID ? "My ID" : "Pepper");
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_MODE, INPUT_PULLUP);
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(drawJpgBlock);
  Wire.begin(OLED_SDA, OLED_SCL);
  Wire.setClock(100000);
  oledReady = oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (!oledReady) Serial.println("OLED not detected at 0x3C");
  drawOLED();
  Serial.println("Cyber ID ready: GPIO6 toggles My ID / Pepper");
}

void loop() {
  if (pressed(modeButton)) switchProfile();
  if (currentProfile != MY_ID) return;
  if ((int32_t)(millis() - nextFrameAt) >= 0) {
    uint32_t started = millis();
    TJpgDec.drawJpg(0, 0, frames[frameIndex], frameSizes[frameIndex]);
    drawFooter("@Jafetdgc");
    frameIndex = (frameIndex + 1) % FRAME_COUNT;
    nextFrameAt = started + FRAME_INTERVAL_MS;
  }
}
