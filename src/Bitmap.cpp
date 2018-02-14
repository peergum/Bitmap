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

#include "Bitmap.h"

Bitmap::Bitmap(Adafruit_SSD1351 *display, const char *filename) {
    _filename = (char *)filename;
    _display = display;
    bmpFile.open(_filename, O_READ);
    Log.info("Bitmap object created for file %s",filename);
}

Bitmap::Bitmap(Adafruit_SSD1351 *display, File bitmap) {
    bmpFile = bitmap;
    _display = display;
    Log.info("Bitmap object created");
}


Bitmap::~Bitmap() {
    Log.info("Bitmap object destroyed");
}

void Bitmap::draw(uint8_t x, uint8_t y) {
    displayWidth = _display->width();
    displayHeight = _display->height();

    // switch display off before loading pic
    if ((x >= displayWidth) || (y >= displayHeight)) {
        return;
    }

    Log.info("Loading image %s", _filename ? _filename : "");

    // Open requested file on SD card
    if (!bmpFile) {
        Log.error("File not found");
        return;
    }

    /*_display->writeCommand(SSD1351_CMD_DISPLAYOFF);*/
    if (!read(x,y)) {
        Log.error("Can't load bitmap");
    };
    // switch display back on
    /*_display->writeCommand(SSD1351_CMD_DISPLAYON);*/
}

bool Bitmap::read(uint8_t x, uint8_t y) {
    uint32_t  buffidx;

    // Parse BMP header
    if(read16() != 0x4D42) { // BMP signature
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

    if ((bmpDepth != 16 && bmpDepth != 24) || read32() != 0) { // 0 = uncompressed
        Log.error("Not a 16 or 24 bits uncompressed bitmap");
        return false;
    }

    Log.info("Image size: %u x %d", bmpWidth, bmpHeight);

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
    if (x+w > displayWidth) {
        w = displayWidth  - x;
    }
    if (y+h > displayHeight) {
        h = displayHeight - y;
    }

    buffidx = sizeof(sdbuffer); // Force buffer reload

    uint16_t color;
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    uint8_t pixelBytes = bmpDepth/8;

    uint16_t *buffer;

    buffer = (uint16_t *)malloc((size_t)(2*w*h));
    if (buffer == NULL) {
        Log.error("Can't allocate %lu bytes.", (unsigned long)2*w*h);
        return false;
    }

    bmpFile.seek(bmpImageoffset);

    for (row=0; row<h; row++) { // For each scanline...

        unsigned long pos;

        if (flip) {// Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + pixelBytes * (bmpHeight - 1 - row) * rowSize;
        } else {    // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + pixelBytes * row * rowSize;
        }
        if (bmpFile.position() != pos) { // Need seek?
            /*Log.info("pos = %lu, wanted = %lu",bmpFile.position(),pos);*/
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
        }

        // optimize by setting pins now
        for (col=0; col<w; col++) { // For each pixel...

            color = 0;
            // Time to read more pixel data?
            if (buffidx+pixelBytes > sizeof(sdbuffer)) { // Indeed
                for (unsigned long i=buffidx; i<sizeof(sdbuffer); i++) {
                    sdbuffer[i-buffidx] = sdbuffer[i];
                }
                bmpFile.read(sdbuffer+sizeof(sdbuffer)-buffidx,
                    buffidx);
                buffidx = 0; // Set index to beginning
            }
            if (bmpDepth == 16) {
                color = ((uint16_t *)sdbuffer)[buffidx/2];
                buffidx+=2;
            } else if (bmpDepth == 24) {
                red = sdbuffer[buffidx++];
                green = sdbuffer[buffidx++];
                blue = sdbuffer[buffidx++];
                color = _display->Color565(blue,green,red);
            }

            //_display->drawPixel(x+col, flip ? y+h-1-row : y+row, color);
            *(buffer + row * w + col) = color;
            /*_display->writeData(color>>8);
            _display->writeData(color);*/
        } // end pixel
    } // end scanline

    _display->writeCommand(SSD1351_CMD_SETCOLUMN);
    _display->writeData(x);
    _display->writeData(x+w-1);
    _display->writeCommand(SSD1351_CMD_SETROW);
    _display->writeData(y);
    _display->writeData(y+h-1);
    _display->writeCommand(SSD1351_CMD_WRITERAM);
    for (int i = 0; i < w * h ; i++) {
        _display->writeData(buffer[i]>>8);
        _display->writeData(buffer[i]);
    }

    Log.info("Loaded in %ld ms",millis() - startTime);
    return true;
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t Bitmap::read16() {
    uint16_t result;
    ((uint8_t *)&result)[0] = read8();
    ((uint8_t *)&result)[1] = read8();
    return result;
}

uint32_t Bitmap::read32() {
    uint32_t result;
    ((uint8_t *)&result)[0] = read8();
    ((uint8_t *)&result)[1] = read8();
    ((uint8_t *)&result)[2] = read8();
    ((uint8_t *)&result)[3] = read8();
    return result;
}

uint8_t Bitmap::read8() {
    uint8_t byte = bmpFile.read();
    /*Serial.printf("[%02X]",byte);*/
    return byte;
}
