CYBER ID - ESP32-S3

Open Cyber_ID_Animation.ino in Arduino IDE. Keep frames.h and name_bitmap.h in same folder.

Install libraries: Adafruit GFX Library, Adafruit ST7735 and ST7789 Library, Adafruit SSD1306, TJpg_Decoder by Bodmer.

This build loops the uploaded video at nominal 10 FPS, without buttons, Pepper still image, or portrait still image. It does not need the separate JPGs at runtime: frames.h embeds all JPEGs in firmware.

GPIO: TFT CS10 DC11 RST12 MOSI13 SCLK14; OLED SDA17 SCL16. TFT BL9 is reserved/not driven. Use the ACTUAL working pins if you rewired them. Disconnect robot servos on 16/17 when testing.

OLED: original cat icon; bitmap JAFET DUARTE, roughly 12px tall; BAT --% placeholder; ELECTRICAL ENGINEER bottom right.

If your TFT is already lit, leave the BL wiring unchanged. Do not drive the BL pin from GPIO before verifying its requirements.

Frame JPGs included in frames_original_resized for reference, not required to compile.

Requires ST7735 TFT and 128x64 SSD1306 OLED as previously assumed.
