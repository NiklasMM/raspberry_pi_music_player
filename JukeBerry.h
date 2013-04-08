// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_JUKEBERRY_H
#define JUKEBERRY_JUKEBERRY_H

#include <string>

#include "./RaspiLCD.h"
#include "./Library.h"

using std::string;

// This is the main class of the JukeBerry Project. It holds an instance of
// classes like RaspiLCD and Library and manages them.
class JukeBerry {
 public:
  // Constructor with the basepath of the library
  JukeBerry(const string& path);

  // this method starts the JukeBerry
  void start();

 private:
  // this method is called once per iteration and handles all the logic
  void update();
  // this method is called once per iteration and handles the drawing on the
  // RaspiLCD
  void draw();

  RaspiLCD _display;
  Library _library;

};

#endif  // JUKEBERRY_JUKEBERRY_H
