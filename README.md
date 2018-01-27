# Bitmap [![Build Status](https://travis-ci.org/peergum/Bitmap.svg?branch=master)](https://travis-ci.org/peergum/Bitmap) [electron+photon]
Bitmap class to load a Windows BMP into a SSD1351 OLED display

## Dependencies
* Adafruit_SSD1351_Photon
* Adafruit_mfGFX
* SdFat

## Prerequisites
* `char *filename` is the path to a *Windows BMP _16bits_ file* on your SDCard
* `Adafruit_SSD1351 display` is your current display

## Usage
At the place you want to load the bitmap (which resides on your SDCard)
```
Bitmap bitmap = Bitmap(display,filename);
bitmap.draw(x,y);
```
Optionally, you can add `delete bitmap;` after loading the bitmap to free the space used by the class.

## Possible future improvements
* Improving readability of picture on OLED display (by possibly adjusting the colors)
* Reading native 24bits or 32bits BMP files
* Reading PNG and JPG files
