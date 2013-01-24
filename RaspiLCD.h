// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>

#include <stdlib.h>
#include <vector>
#include "./definitions.h"
#include "./font_terminal_6x8.inc"

using std::vector;

class RaspiLDC {
 public:
  // default constuctor
  RaspiLDC();

  // clear display
  void clear();

  // writes a string to the display
  void printLine(size_t x0, size_t, char *s);

  // write framebuffer to display
  void writeFramebuffer();

  // Setters
  void setBacklight(bool value);

 private:
 
  // initializes the display
  int initialize();

  // get Hardware revison of the pi to find the correct
  // pin assignment
  // for more info see: http://www.raspberrypi.org/archives/1929
  static size_t GetRaspberryHwRevision();


  void putPixel(size_t x, size_t y, size_t color);

  // send data byte to display
  void lcd_write_data(uint8 d);
  // send command byte to display
  void lcd_write_cmd(uint8 d);

  // emulate SPI on GPIO
  void spiPutc(unsigned char d);

  // set position for output
  void lcd_set_xy(uint8 x,uint8 ypage);
 
  // the pixels valiues
  vector<uint16>	_pixels;

  // the backlight state (on or off)
  bool _backlight;

  // the currently selected font
  const unsigned char *_font;

  // the framebuffer
  vector<vector<size_t> > _framebuffer;

  // button pins
  vector<uint8> _buttonPins;

  // button status variables
  uint8	_button;
  uint8 _buttonPressed;
  uint8 _buttonMem;

  // CONSTANTS
  static const size_t PIN_LCD_BACKLIGHT	= 18;
  static const size_t PIN_LCD_RST = 25;
  static const size_t PIN_LCD_CS = 8;
  static const size_t PIN_LCD_RS = 7;
  static const size_t PIN_LCD_MOSI = 10;
  static const size_t PIN_LCD_SCLK = 11;

  static const size_t LCD_WIDTH = 128;
  static const size_t LCD_HEIGHT = 64;
  static const size_t LCD_X_OFFSET = 4;
};
