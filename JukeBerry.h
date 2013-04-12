// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_JUKEBERRY_H
#define JUKEBERRY_JUKEBERRY_H

#include <string>
#include <vector>
#include <map>

#include "./RaspiLCD.h"
#include "./Library.h"
#include "./Player.h"
#include "./Screen.h"

class Screen;

using std::string;
using std::vector;
using std::map;

enum ScreenEnum {SC_MainMenu, SC_Library, SC_Playlist};

// This is the main class of the JukeBerry Project. It holds an instance of
// classes like RaspiLCD and Library and manages them.
class JukeBerry {
 public:
  // Constructor with the basepath of the library
  JukeBerry(const string& path);

  // this method starts the JukeBerry
  void start();

  // this method terminates the program
  void requestTermination();

  // changes to the given screen
  void changeToScreen(ScreenEnum screen);

 private:
  // this method is called once per iteration and handles all the logic
  void update();
  // this method is called once per iteration and handles the drawing on the
  // RaspiLCD
  void draw();

  RaspiLCD _display;
  Library _library;
  Player _player;

  // a list of all screens
  map<ScreenEnum, Screen*> _screens;
  // the currently active screen
  ScreenEnum _activeScreen;

  // if true the program terminates
  // is set in stop()
  bool _terminate;

};

#endif  // JUKEBERRY_JUKEBERRY_H
