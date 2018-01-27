/***************************************************
    Bitmap class for SSD1351 display

    This is an adaptation by Philippe Hilger ("PeerGum")
    of the original code written by Limor Fried/Ladyada
    for Adafruit Industries.

    GPL v.3 licensed.

    For the Photon or Electron, the Adafruit_mfGFX and the
    Adafruit_SSD1351_Photon libraries are also required

    Be sure to install them!

 ****************************************************/

#include "Bitmap.h"

Bitmap::Bitmap(Adafruit_SSD1351 *display, char *filename) {
    _filename = filename;
    _display = display;
    Log.info("Bitmap object created for file %s",filename);
}

Bitmap::~Bitmap() {
    Log.info("Bitmap object destroyed");
}

void Bitmap::draw(uint8_t x, uint8_t y) {

    // switch display off before loading pic
    _display->writeCommand(SSD1351_CMD_DISPLAYOFF);
    if ((x >= _display->width()) || (y >= _display->height())) {
        return;
    }

    Log.info("Loading image %s", _filename);

    // Open requested file on SD card
    if (!bmpFile.open(_filename)) {
        Log.error("File not found");
        return;
    }

    if (!read(x,y)) {
        Log.error("Can't load bitmap");
    };
    bmpFile.close();
    // switch display back on
    _display->writeCommand(SSD1351_CMD_DISPLAYON);
}

bool Bitmap::read(uint8_t x, uint8_t y) {
    // Parse BMP header
    if(!read16() == 0x4D42) { // BMP signature
        Log.error("Not a BMP");
        return false;
    }

    Log.info("File size: %lu",read32());
    (void)read32(); // Read & ignore creator bytes

    bmpImageoffset = read32(); // Start of image data
    Log.info("Image Offset: %lu", bmpImageoffset);

    // Read DIB header
    Log.info("Header size: %lu", read32());
    bmpWidth  = read32();
    bmpHeight = read32();

    if(!read16() == 1) { // # planes -- must be '1'
        Log.error("Number of planes should be '1'");
        return false;
    }

    bmpDepth = read16(); // bits per pixel
    Log.info("Bit Depth: %u", bmpDepth);

    if (bmpDepth != 16 || read32() != 0) { // 0 = uncompressed
        Log.error("Not a 16 bits uncompressed bitmap");
        return false;
    }

    Log.info("Image size: %lu x %lu", bmpWidth, bmpHeight);

    // BMP rows are padded (if needed) to 4-byte boundary
    rowSize = 4*((bmpWidth-1)/4+1);

    // If bmpHeight is negative, image is in top-down order.
    // This is not canon but has been observed in the wild.
    if (bmpHeight < 0) {
        bmpHeight = -bmpHeight;
        flip      = false;
    }

    // Crop area to be loaded
    w = bmpWidth;
    h = bmpHeight;
    if (x+w > _display->width()) {
        w = _display->width()  - x;
    }
    if (y+h > _display->height()) {
        h = _display->height() - y;
    }

    bmpFile.seekSet(bmpImageoffset);

    buffidx = sizeof(sdbuffer); // Force buffer reload

    for (row=0; row<h; row++) { // For each scanline...
        /*_display->goTo(x, flip ? y+h-1-row : y+row);*/

        // optimize by setting pins now
        for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                buffidx = 0; // Set index to beginning
            }

            uint16_t color = ((uint16_t)sdbuffer[buffidx++]);
            color |= ((uint16_t)sdbuffer[buffidx++])<<8;
            /*Serial.printf("[%04X]",color);*/
            _display->drawPixel(x+col, flip ? y+h-1-row : y+row, color);
        } // end pixel
        Serial.println();
    } // end scanline
    Log.info("Loaded in %ld ms",millis() - startTime);
    return true;
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t Bitmap::read16() {
    uint16_t result;
    ((uint8_t *)&result)[0] = read8(); // LSB
    ((uint8_t *)&result)[1] = read8(); // MSB
    return result;
}

uint32_t Bitmap::read32() {
    uint32_t result;
    ((uint8_t *)&result)[0] = read8(); // LSB
    ((uint8_t *)&result)[1] = read8();
    ((uint8_t *)&result)[2] = read8();
    ((uint8_t *)&result)[3] = read8(); // MSB return result;
    return result;
}

uint8_t Bitmap::read8() {
    uint8_t byte = bmpFile.read();
    /*Serial.printf("[%02X]",byte);*/
    return byte;
}
