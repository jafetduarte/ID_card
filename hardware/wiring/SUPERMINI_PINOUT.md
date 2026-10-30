# Cyber ID — ESP32-S3 SuperMini GPIO assignment

**Prototype pin plan; not yet hardware-verified.**

| Device | Screen pin | ESP32-S3 GPIO |
|---|---|---|
| Large ST7735 TFT | CS | GPIO1 |
| Large ST7735 TFT | DC / A0 | GPIO2 |
| Large ST7735 TFT | RES / RST | GPIO3 |
| Large ST7735 TFT | SDA / DIN / MOSI | GPIO4 |
| Large ST7735 TFT | SCL / SCK | GPIO5 |
| Button 1 (future) | Mode | GPIO6 |
| Small SSD1306 OLED | SDA | GPIO12 |
| Small SSD1306 OLED | SCL | GPIO13 |

GPIO8–11 unused for now. TFT SCL is SPI clock; OLED SCL is I2C clock. 

**Power:** 

**Assumptions:** 128x160 ST7735 TFT using `INITR_BLACKTAB`; 128x64 SSD1306 OLED, I2C address 0x3C. Confirm actual modules and backlight circuitry. Buttons are only reserved, not implemented or wired. The salvaged touch-controller outputs must not be connected directly to GPIO without electrical verification.

## Physical order

```
                 USB-C
             +-----------+
         TX  | o       o | 5V
         RX  | o       o | GND
 TFT CS   1  | o       o | 3V3
 TFT DC   2  | o       o | 13 OLED SCL
 TFT RES  3  | o       o | 12 OLED SDA
 TFT SDA  4  | o       o | 11
 TFT SCL  5  | o       o | 10
 MODE     6  | o       o | 9
          7  | o       o | 8
             +-----------+
```
