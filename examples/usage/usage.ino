// Test code

#include "Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon.h"
#include "SdFat.h"
#include "Bitmap.h"

#define OLED_SD_SCLK A3
#define OLED_SD_DI  A5
#define SD_DO       A4
#define SD_CS       A2
#define OLED_RST    D1
#define OLED_CS     D5
#define OLED_DC     D0

// Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Adafruit_SSD1351 display(OLED_CS, OLED_DC, OLED_SD_DI, OLED_SD_SCLK, OLED_RST);

void setup() {
    Bitmap bitmap = Bitmap(&display,"my_bitmap.bmp");
    bitmap.draw(0,0);
}

void loop() {
    // do nothing :P
}
