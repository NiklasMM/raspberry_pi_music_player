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
  
}
