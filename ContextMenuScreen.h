// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_CONTEXTMENUSCREEN_H_
#define JUKEBERRY_CONTEXTMENUSCREEN_H_

#include "./Screen.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class ContextMenuScreen : public Screen {
 public:
  // Contructor taking a list of entries to choose from
  // and a pointer to the variable the chosen entry will be put into
  ContextMenuScreen(RaspiLCD& display, JukeBerry* jb,
                    const vector<string>& options,
                    int* result);
  // implementation of the update method
  virtual void update();
  // implementation of the draw method
  virtual void draw();

 private:
  // a pointer to the variable the result will be stored in
  int* _result;

  // currently selected entry
  int _selectedEntry;

  // all entries
  vector<string> _entries;
  
};

#endif  // JUKEBERRY_CONTEXTMENUSCREEN_H_
