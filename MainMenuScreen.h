// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_MAINMENUSCREEN_H_
#define JUKEBERRY_MAINMENUSCREEN_H_

#include <vector>
#include <string>

#include "./Screen.h"
#include "./RaspiLCD.h"

using std::string;
using std::vector;

// The Main Menu in Jukeberry which is displayed at startup
class MainMenuScreen : public Screen {
  public:
  MainMenuScreen(RaspiLCD& display, JukeBerry* jb);
  // implementation of the update method
  virtual void update();
  // implementation of the draw method
  virtual void draw();
 private:
  // all available menu entries
  vector<string> _menuEntries;
  // the currently selected menu entry
  size_t _selectedEntry;
};

#endif  // JUKEBERRY_MAINMENUSCREEN_H_
