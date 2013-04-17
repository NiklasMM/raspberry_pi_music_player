// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include "./ContextMenuScreen.h"

#include <string>
#include <vector>

using std::vector;
using std::string;


// _____________________________________________________________________________
ContextMenuScreen::ContextMenuScreen(RaspiLCD& display, JukeBerry* jb,
                                     const vector<string>& options,
                                     int* result) :
  Screen(display, jb), _result(result), _selectedEntry(0) {

  // save entries
  for (size_t i = 0; i < options.size(); i++) {
    _entries.push_back(options[i]);
  }
}

// _____________________________________________________________________________
void ContextMenuScreen::draw() {
  _raspiLcd.drawRectangle(32, 20, 127, 63, 1, COLOR_WHITE, COLOR_BLACK);
  std::cout << "draw\n";
  _raspiLcd.printList(33, 21, _entries, _selectedEntry);
}

// _____________________________________________________________________________
void ContextMenuScreen::update() {
  if (_raspiLcd.buttonPressed(DOWN)) {
    _selectedEntry++;
    if (_selectedEntry >= static_cast<int>(_entries.size())) _selectedEntry = 0;
    if (_selectedEntry < 0) _selectedEntry = _entries.size() - 1;
  }
  if (_raspiLcd.buttonPressed(CENTER)) *_result = _selectedEntry;
}
