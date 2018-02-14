# Bitmap [![Build Status](https://travis-ci.org/peergum/Bitmap.svg?branch=master)](https://travis-ci.org/peergum/Bitmap) [electron+photon]
Bitmap class to load a Windows BMP into a SSD1351 OLED display

## Features
* Reads 16/24 bits Windows BMP (uncompressed)
* Compatible with Adafruit_SSD1351 driver

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
    ...
    SdFile bmpFile;
    bmpFile.open(filename, O_READ);
    Bitmap bitmap2 = Bitmap(&display,bmpFile);
    bitmap2.draw(x,y);
}
```

## Notes
* image is loaded into memory first, then written to display ram through SPI
* because of the use of display SPI commands, the code won't work as is with another type of display
* if you use custom fonts (e.g. Arial8) within your project, you have to copy this library locally and update _fonts.h_ accordingly

## Possible future improvements
* Reading compressed formats
* Reading 32bits BMP files
* Reading PNG and JPG files
* use callbacks for display off/on, to make it compatible with the GFX lib, rather than the hardware display driver

## changelog
* 0.0.2: removed serial linefeeds sent while loading the picture
* 0.0.3: color fix (0.0.2 was not working) + handling 24 bits bitmaps + loading pictures in memory, then writing to display ram directly
* 0.0.4: fixed dependencies
* 0.0.5: fixed R,G,B order inversion in 24bits bitmaps :/
