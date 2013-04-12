// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include "./MainMenuScreen.h"
#include "./RaspiLCD.h"


// _____________________________________________________________________________
MainMenuScreen::MainMenuScreen(RaspiLCD& display, JukeBerry* jb) :
  Screen(display, jb), _selectedEntry(0) {
    // initialize Menu Options
    _menuEntries.push_back("Library Browser");
    _menuEntries.push_back("Exit");
}


// _____________________________________________________________________________
void MainMenuScreen::update() {
  // update the selected entry
  if (_raspiLcd.buttonPressed(DOWN)) _selectedEntry++;
  if (_raspiLcd.buttonPressed(UP)) _selectedEntry = (_selectedEntry == 0)?
                                   _menuEntries.size() - 1:_selectedEntry - 1;
  // wrap around
  if (_selectedEntry >= _menuEntries.size()) _selectedEntry = 0;

  if (_raspiLcd.buttonPressed(CENTER)) {
    switch(_selectedEntry) {
      case 0:
        _jukeBerry->changeToScreen(SC_Library);
        break;
      case 1:
        _jukeBerry->requestTermination();
        break;
      default:
        break;
    }
  }
}

// _____________________________________________________________________________
void MainMenuScreen::draw() {
  _raspiLcd.printList(_menuEntries, _selectedEntry);
}
