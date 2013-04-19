// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_PLAYERSCREEN_H_
#define JUKEBERRY_PLAYERSCREEN_H_

#include "./Screen.h"
#include "./RaspiLCD.h"
#include "./JukeBerry.h"

class PlayerScreen : public Screen {
 public:
  // Contructor taking a list of entries to choose from
  // and a pointer to the variable the chosen entry will be put into
  PlayerScreen(RaspiLCD& display, JukeBerry* jb, Player& player);
  // implementation of the update method
  virtual void update();
  // implementation of the draw method
  virtual void draw();

 private:
  // a reference to the Player instance
  Player& _player;
};

#endif  // JUKEBERRY_PLAYERSCREEN_H_
