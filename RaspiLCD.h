// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_RASPILCD_H_
#define JUKEBERRY_RASPILCD_H_

#include <stdlib.h>
#include <vector>
#include <string>
#include "./font_terminal_6x8.inc"

using std::vector;
using std::string;

enum RaspiLcdButtons {UP, DOWN, LEFT, RIGHT, CENTER};

enum RaspiLcdColors {COLOR_TRANSPARENT = -1, COLOR_WHITE = 0, COLOR_BLACK = 1};

class RaspiLCD {
 public:
  // default constuctor
  RaspiLCD();

  // clear display
  void clear();

  // updates the button state
  void update();

  // writes a list of strings to the display
  // one string per line starting at (x, y)
  void printList(size_t x, size_t y,
                 const vector<string>& lines, int selected = -1);

  // print a single string to the display
  void printString(size_t x, size_t y, const string& input);

  // draws a line from (x1, y1) to (x2, y2)
  void drawLine(size_t x1, size_t y1, size_t x2, size_t y2,
                RaspiLcdColors color);

  // Draws a rectangle using the given fill an line color
  void drawRectangle(size_t x1, size_t y1, size_t x2, size_t y2,
                     size_t lineWidth, RaspiLcdColors fillColor,
                     RaspiLcdColors lineColor);

  // actually draw the stuff in the buffer to the screen
  void drawBuffer() const;

  // clears the buffer
  void clearBuffer();

  // Getters
  size_t getMaxCharactersPerLine() const { return MAX_CHARS_PER_LINE; }

  // Setters
  void setBacklight(bool value);

  // returns true if the button was pressed
  bool buttonPressed(RaspiLcdButtons button) const;

  // clears the screen and switches the light of
  void shutdown();

 private:
  // the backlight state (on or off)
  bool _backlight;

  // the currently selected font
  const unsigned char *_font;

  // the framebuffer
  vector<vector<size_t> > _framebuffer;

  // button states (0 = not pressed, 1 = pressed)
  int _buttonUp;
  int _buttonDown;
  int _buttonLeft;
  int _buttonRight;
  int _buttonCenter;

  // CONSTANTS
  static const size_t LCD_WIDTH = 128;
  static const size_t LCD_HEIGHT = 64;
  static const size_t LCD_X_OFFSET = 4;

  static const size_t MAX_CHARS_PER_LINE = 20;
  static const size_t MAX_LINES_DISPLAYED = 8;
};

#endif // JUKEBERRY_RASPILCD_H_
