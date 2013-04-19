// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_SCREEN_H_
#define JUKEBERRY_SCREEN_H_

#include "./RaspiLCD.h"
#include "./JukeBerry.h"

class JukeBerry;

// This class is the base class for all screens displayed on the RaspiLCD display
// such as a Playlist, Library browser and such...
// Each screen must implement an update and a draw method
class Screen {
 public:
  Screen(RaspiLCD& display, JukeBerry* jb);
  // a virtual destructor
  ~Screen();
  // the update method which handles all the logic
  virtual void update() = 0;
  // the draw method which handles all drawing on the
  virtual void draw() = 0;
 protected:
  // a reference to the RaspiLCD class used for button state queries and
  // drawing
  RaspiLCD& _raspiLcd;

  // nicely prints a title to the LCD, that is the passed String centered
  // on top of the screen and a divider line below it
  void printTitle(const string& title) const;

  JukeBerry* _jukeBerry;
};


#endif  // JUKEBERRY_SCREEN_H_
