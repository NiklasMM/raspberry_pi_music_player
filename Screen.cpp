// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include "./Screen.h"

// _____________________________________________________________________________
Screen::Screen(RaspiLCD& display, JukeBerry* jb) :
  _raspiLcd(display), _jukeBerry(jb){ }

// _____________________________________________________________________________
Screen::~Screen() {}

// _____________________________________________________________________________
void Screen::printTitle(const string& title) const {
  // get the strings width
  size_t width = _raspiLcd.getStringWidth(title);
  // calculate x position such that the string is centered
  size_t x = (_raspiLcd.getWidth()/2) - (width/2);

  // print the string
  // if the string fits to the screen center
  if (title.length() <= _raspiLcd.getMaxCharactersPerLine()) {
    _raspiLcd.printString(x, 0, title);
  } else {
    // otherwise only display first MAX_CHAR_PER_LINE characters
    _raspiLcd.printString(0, 0, title.substr(0, _raspiLcd.getMaxCharactersPerLine()));
  }
  // print divider line
  _raspiLcd.drawLine(0,9,128,9, COLOR_BLACK);
}
