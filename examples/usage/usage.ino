// Test code
SYSTEM_MODE(MANUAL);

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
SdFatSoftSpi<SD_DO,OLED_SD_DI,OLED_SD_SCLK> SD;

SerialLogHandler logHandler(9600,LOG_LEVEL_WARN, {
    {"app", LOG_LEVEL_INFO},
    {"system", LOG_LEVEL_INFO}
});

void setup() {
    Serial.begin(9600); // USB monitor
    delay(5000);
    Log.info("Start");
    display.begin();
    delay(50);
    if (SD.begin(SD_CS)) {
        Log.info("SD Card initialized.");
        //File bmpFile("my_bitmap.bmp",O_READ);
        Bitmap bitmap = Bitmap(&display,"my_bitmap.bmp");
        bitmap.draw(0,0);
        display.writeCommand(SSD1351_CMD_DISPLAYON);
    } else {
        Log.error("SD Init failed");
    }
}

void loop() {
    // do nothing :P
}
