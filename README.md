# Bitmap [![Build Status](https://travis-ci.org/peergum/Bitmap.svg?branch=master)](https://travis-ci.org/peergum/Bitmap) [electron+photon]
Bitmap class to load a Windows BMP into a SSD1351 OLED display

## Dependencies
* Adafruit_SSD1351_Photon
* Adafruit_mfGFX
* SdFat

## Usage
```
char *filename` // path to a Windows BMP _16bits_ file on your SDCard
Adafruit_SSD1351 display(...) // your current display

setup() {
    Bitmap bitmap = Bitmap(&display,filename);
    bitmap.draw(x,y);
    delete bitmap; //optional: free the space used by the class
}
```

## Notes
* image is not loaded into memory, but directly to the display
* display is switched OFF before loading, and ON after, so the picture appears at once
* because of the use of display SPI commands (off/on), the code won't work as is with another type of display

## Possible future improvements
* Improving readability of picture on OLED display (by possibly adjusting the colors)
* Reading native 24bits or 32bits BMP files
* Reading PNG and JPG files
* use callbacks for display off/on, to make it compatible with the GFX lib, rather than the hardware display driver
