# Cyber ID

A customizable electronic ID badge built using an ESP32-S3 SuperMini, featuring two displays, animated graphics, and switchable profiles.

This project explores embedded programming, electronics, and hardware prototyping.

## Features

- **Dual displays:** ST7735 TFT for images and animations, SSD1306 OLED for text and icons.
- **Animated ID:** Plays a custom animation alongside my name and engineering title.
- **Multiple profiles:** Switches between my personal ID and Pepper's profile.
- **Single-button control:** One push button cycles between profiles.
- **Compact design:** Uses an ESP32-S3 SuperMini to reduce the overall size.

## Hardware

- ESP32-S3 SuperMini
- ST7735 TFT display (128 × 160)
- SSD1306 OLED display (128 × 64)
- Momentary push button
- Breadboard and jumper wires

### GPIO Connections

| Component | ESP32-S3 GPIO |
|---|---|
| TFT CS | GPIO1 |
| TFT DC | GPIO2 |
| TFT RST | GPIO3 |
| TFT MOSI | GPIO4 |
| TFT SCLK | GPIO5 |
| Mode Button | GPIO6 |
| OLED SDA | GPIO12 |
| OLED SCL | GPIO13 |

The mode button connects between GPIO6 and GND.

[Detailed Wiring Documentation](hardware/wiring/SUPERMINI_PINOUT.md)

## Software

Developed using Arduino IDE and C++.

**Required Libraries:**
- Adafruit GFX
- Adafruit ST7735 and ST7789
- Adafruit SSD1306
- TJpg_Decoder

**Main Firmware:**

[Cyber_ID_SuperMini.ino](firmware/code/Cyber_ID_SuperMini/Cyber_ID_SuperMini.ino)

The animation and Pepper image are embedded in the firmware as header files.

## Development Progress

### Prototype V0.1 — October 26, 2026

Initial breadboard prototype using an ESP32-S3-CAM with two displays.

[View V0.1 Photos](images/versions/v.01/)

### Prototype V0.2 — October 29, 2026

Switched to the smaller ESP32-S3 SuperMini and integrated a working button to switch between profiles.

[View V0.2 Photos](images/versions/v.02/)

## Current Status

**V0.2 — Functional Breadboard Prototype**

Both displays and profile switching are working.

Next steps include integrating a rechargeable power supply, voltage regulation, and developing a compact enclosure.

## Documentation

[Engineering Log](docs/ENGINEERING_LOG.md)

[Hardware Documentation](hardware/)

[Firmware Assets](firmware/assets/)

## Future Plans

- Rechargeable battery-powered operation
- Compact wearable enclosure
- Additional customizable ID profiles
- NFC integration