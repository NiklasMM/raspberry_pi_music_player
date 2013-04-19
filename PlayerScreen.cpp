// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)


#include "./PlayerScreen.h"
#include "./Screen.h"
#include "./RaspiLCD.h"
#include "./JukeBerry.h"

// _____________________________________________________________________________
PlayerScreen::PlayerScreen(RaspiLCD& display, JukeBerry* jb, Player& player) :
  Screen(display, jb), _player(player) {
  
}


// _____________________________________________________________________________
void PlayerScreen::draw() {
  
}

// _____________________________________________________________________________
void PlayerScreen::update() {

}
