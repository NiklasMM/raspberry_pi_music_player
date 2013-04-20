// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)


#include <string>

#include "./PlayerScreen.h"
#include "./Screen.h"
#include "./RaspiLCD.h"
#include "./JukeBerry.h"

using std::string;

// _____________________________________________________________________________
PlayerScreen::PlayerScreen(RaspiLCD& display, JukeBerry* jb, Player& player) :
  Screen(display, jb), _player(player) {
  
}


// _____________________________________________________________________________
void PlayerScreen::draw() {
  printTitle("Player");

  _raspiLcd.printString(0, 12, "now playing:");

  string currentSong;
  if (_player.getCurrentSong(currentSong)) {
    _raspiLcd.printString(5, 20, currentSong);
  } else {
    _raspiLcd.printString(5, 20, "nothing");
  }
}

// _____________________________________________________________________________
void PlayerScreen::update() {
  if (_raspiLcd.buttonPressed(LEFT)) {
    _jukeBerry->changeToScreen(SC_MainMenu);
    return;
  }
}
