// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)


#include <string>
#include <thread>

#include "./bcm2835.h"
#include "./JukeBerry.h"
#include "./RaspiLCD.h"
#include "./Library.h"
#include "./LibraryBrowserScreen.h"

// _____________________________________________________________________________
JukeBerry::JukeBerry(const string& path) :
  _library(Library(path)){
  // create a new LibraryBrowserScreen and set it as active
  _screens.push_back(new LibraryBrowserScreen(_display, _library, _player));
  _activeScreen = 0;
}

// _____________________________________________________________________________
void JukeBerry::start() {
  // enter an infinite loop of update and draw with sleeping in between
  while(1) {
    update();
    draw();
    bcm2835_delay(100);
  }
}

// _____________________________________________________________________________
void JukeBerry::update() {
  _display.update();

  // update the active screen
  _screens[_activeScreen]->update();
}

// _____________________________________________________________________________
void JukeBerry::draw() {
  // draw the active screen
  _screens[_activeScreen]->draw();
  // draw what's in the buffer
  _display.drawBuffer();
}
