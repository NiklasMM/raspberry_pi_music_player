// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_SCREEN_H_
#define JUKEBERRY_SCREEN_H_

#include "./RaspiLCD.h"

// This class is the base class for all screens displayed on the RaspiLCD display
// such as a Playlist, Library browser and such...
// Each screen must implement an update and a draw method
class Screen {
 public:
  Screen(RaspiLCD& display);
  // the update method which handles all the logic
  virtual void update() = 0;
  // the draw method which handles all drawing on the
  virtual void draw() = 0;
 protected:
  // a reference to the RaspiLCD class used for button state queries and
  // drawing
  RaspiLCD& _raspiLcd;
};


#endif  // JUKEBERRY_SCREEN_H_
