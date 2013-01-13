// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>

#include <stdlib.h>
#include <vector>
#include "./definitions.h"

using std::vector;

class Display {
 public:
  // default constuctor
  Display();

  // clear display
  void clear();

  // Setters
  void setBacklight(bool value);

 private:
  // initializes the display
  int initialize();

  // get Hardware revison of the pi to find the correct
  // pin assignment
  // for more info see: http://www.raspberrypi.org/archives/1929
  static size_t GetRaspberryHwRevision();
 
  // the pixels valiues
  vector<uint16>	_pixels;

  // the backlight state (on or off)
  bool _backlight;

  // RaspiLCD specific constants
  static const size_t PIN_LCD_BACKLIGHT	= 18;
  static const size_t PIN_LCD_RST = 25;
  static const size_t PIN_LCD_CS = 8;
  static const size_t PIN_LCD_RS = 7;
  static const size_t PIN_LCD_MOSI = 10;
  static const size_t PIN_LCD_SCLK = 11;

  // button pins
  vector<uint8> _buttonPins;

  // button status variables
  uint8	_button;
  uint8 _buttonPressed;
  uint8 _buttonMem;
};
