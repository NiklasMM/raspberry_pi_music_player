// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_LIBRARYBROWSERSCREEN_H_
#define JUKEBERRY_LIBRARYBROWSERSCREEN_H_

#include <chrono>
#include <memory>

#include "./Library.h"
#include "./RaspiLCD.h"
#include "./ContextMenuScreen.h"
#include "./Screen.h"
#include "./Player.h"


// This class represents a Screen that is used to browse the library and select
// songs to play
class LibraryBrowserScreen: public Screen {
 public:
  LibraryBrowserScreen(RaspiLCD& display, JukeBerry* jb,  Library& lib, Player& player);
  // implementation of the update method
  virtual void update();
  // implementation of the draw method
  virtual void draw();
 private:
  // requests the files in the current directory from the Library and
  // updates _currentFiles
  void updateCurrentFiles();
 
  Library& _library;
  Player& _player;

  // the files in the current directory
  vector<string> _currentFiles;
  // the displayed version of those files
  vector<string> _displayedFiles;
  // the currently selected file
  size_t _selectedFile;


  // the last time a scrolling iteration occured
  std::chrono::high_resolution_clock::time_point _lastScrollTime;
  // the last position
  size_t _lastScrollPos;

  std::unique_ptr<ContextMenuScreen> _contextMenu;
  int _contextMenuReturnValue;
};

#endif // JUKEBERRY_LIBRARYBROWSERSCREEN_H_
