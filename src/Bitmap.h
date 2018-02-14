/***************************************************
    Bitmap class for SSD1351 display

    This is an adaptation by Philippe Hilger ("PeerGum")
    of the original code written by Limor Fried/Ladyada
    for Adafruit Industries.

    GNU General Public License v3.0

    For the Photon or Electron, the Adafruit_mfGFX and the
    Adafruit_SSD1351_Photon libraries are also required

    Be sure to install them!

 ****************************************************/

#ifndef Bitmap_h
#define Bitmap_h

#include "application.h"
#include "Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon.h"
#include "SdFat.h"

#define BUFFPIXEL 36

class Bitmap {
public:
    Bitmap(Adafruit_SSD1351 *display, const char *filename);
    Bitmap(Adafruit_SSD1351 *display, File bmpFile);
    void draw(uint8_t x, uint8_t y);
    ~Bitmap(void);

private:
    bool read(uint8_t x, uint8_t y);
    uint16_t read16(void);
    uint32_t read32(void);
    uint8_t read8(void);

    Adafruit_SSD1351 *_display;
    char *_filename = NULL;
    File bmpFile;
    uint8_t bmpWidth, bmpHeight;   // W+H in pixels
    uint8_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
    boolean  flip    = true;        // BMP is stored bottom-to-top
    int      w, h, row, col;
    uint8_t  r, g, b;
    uint32_t pos = 0, startTime = millis();
    uint8_t displayWidth, displayHeight;
};

#endif
