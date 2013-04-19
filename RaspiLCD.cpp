// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include "./RaspiLCD.h"
#include "./bcm2835.h"

#include "./cFunctions.h"

using std::ifstream;
using std::string;

// _____________________________________________________________________________
RaspiLCD::RaspiLCD()
 : _backlight(false), _buttonUp(0), _buttonDown(0), _buttonLeft(0), _buttonRight(0),
   _buttonCenter(0) {

   // initialize the display
  if(!RaspiLcdHwInit()){
    std::cerr << "RaspiLcdHwInit() failed!\r\n";
  }

  LCD_Init();			    // Initialize Display
  setBacklight(true);	// Turn Backlight on
}

// _____________________________________________________________________________
void RaspiLCD::setBacklight(bool value) {
  _backlight = value;

  if (_backlight) {
    SetBacklight(1);
  } else {
    SetBacklight(0);
  }
}

// _____________________________________________________________________________
void RaspiLCD::clear() {
  // iterate over entries in the framebuffer and zero them
  for (size_t i = 0; i < _framebuffer.size(); i++)
    for (size_t j = 0; j < _framebuffer.size(); j++)
      _framebuffer[i][j] = 0;
}

// _____________________________________________________________________________
void RaspiLCD::printList(size_t x, size_t y,
                         const vector<string>& lines, int selected) {
	LCD_SetFont(0);

  // calculate available lines and collumns at this position
  size_t linesDisplayed = MAX_LINES_DISPLAYED - (y/8);
  size_t collumns = MAX_CHARS_PER_LINE - (x/6);

  // iterate over all lines
  for (size_t i = 0; i < lines.size(); i++) {
    // stop if more than MAX_LINES_DISPLAYED lines are present
    if (i >= linesDisplayed) break;

    const string& line = lines[i];

    string displayedString = line;

    // if a string is longer than the display is wide, use horizontal scrolling
    // to make the whole name readable
    if (displayedString.length() > collumns) {
      displayedString = displayedString.substr(0, collumns);
    }
    // print a ">" in front of the selected file
    if (selected == static_cast<int>(i)) {
      displayedString = ">" + displayedString;
    } else {
      displayedString = " " + displayedString;
    }
    LCD_PrintXY(0 + x,8*i-1 + 2 + y,const_cast<char*>(displayedString.c_str()));
  }
}

// _____________________________________________________________________________
void RaspiLCD::update() {
  // update buttons
  UpdateButtons();
  // reset button states
  _buttonUp = 0; _buttonDown = 0; _buttonLeft = 0; _buttonRight = 0; _buttonCenter = 0;

  getButtonStates(&_buttonUp, &_buttonDown, &_buttonLeft, &_buttonRight, &_buttonCenter);
}


// _____________________________________________________________________________
bool RaspiLCD::buttonPressed(RaspiLcdButtons button) const {
  switch (button) {
    case UP:
      return (_buttonUp == 1);
      break;
    case DOWN:
      return (_buttonDown == 1);
      break;
    case LEFT:
      return (_buttonLeft == 1);
      break;
    case RIGHT:
      return (_buttonRight == 1);
    case CENTER:
      return (_buttonCenter == 1);
      break;
    default:
      return false;
  }
}

// _____________________________________________________________________________
void RaspiLCD::drawBuffer() const {
  LCD_WriteFramebuffer();
}

void RaspiLCD::clearBuffer() {
  LCD_ClearScreen();
}

// _____________________________________________________________________________
void RaspiLCD::shutdown() {
  LCD_ClearScreen();
  LCD_WriteFramebuffer();
  _backlight = false;
  SetBacklight(0);
}

// _____________________________________________________________________________
void RaspiLCD::drawLine(size_t x1, size_t y1, size_t x2, size_t y2,
                        RaspiLcdColors color) {
  // check if the points are inside the display
  if (x1 > 128 || x2 > 128 || y1 > 64 || y2 > 64) return;
  LCD_SetPenColor(color);
  LCD_DrawLine(x1, y1, x2, y2);
}

// _____________________________________________________________________________
void RaspiLCD::printString(size_t x, size_t y, const string& input) {
  // check if the point is in the display
  if (x > 128 || y > 64) return;

  // calculate max characters at this position
  size_t maxChars = MAX_CHARS_PER_LINE - (x/6);

  string displayedString = input.substr(0, (input.length() >= maxChars)?maxChars:input.length());

  LCD_PrintXY(x, y,const_cast<char*>(displayedString.c_str()));

}

// _____________________________________________________________________________
void RaspiLCD::drawRectangle(size_t x1, size_t y1, size_t x2, size_t y2,
                             size_t lineWidth, RaspiLcdColors fillColor,
                             RaspiLcdColors lineColor) {
  LCD_SetPenColor(lineColor);
  LCD_SetFillColor(fillColor);

  LCD_DrawRect(x1, y1, x2, y2, lineWidth);
}
