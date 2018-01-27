/*
 * Project mc3wp-reader
 * Description: Electron firmware to read/store/upload data
 *  from a Troll MC III WP Flow Analyzer
 * Author: Philippe Hilger
 * Date: June 2017
 */

#ifndef Bitmap_h
#define Bitmap_h

#include "application.h"
#include "SdFat.h"
#include "Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon.h"

#define BUFFPIXEL 130

class Bitmap {
public:
    Bitmap(Adafruit_SSD1351 *display, char *filename);
    void draw(uint8_t x, uint8_t y);
    ~Bitmap(void);

private:
    bool read(uint8_t x, uint8_t y);
    uint16_t read16(void);
    uint32_t read32(void);
    uint8_t read8(void);

    Adafruit_SSD1351 *_display;
    char *_filename;
    SdFile     bmpFile;
    uint8_t      bmpWidth, bmpHeight;   // W+H in pixels
    uint8_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbuffer[2*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
    uint32_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    boolean  goodBmp = false;       // Set to true on valid header parse
    boolean  flip    = true;        // BMP is stored bottom-to-top
    int      w, h, row, col;
    uint8_t  r, g, b;
    uint32_t pos = 0, startTime = millis();
};

#endif
