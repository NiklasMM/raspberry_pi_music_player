// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include "./LibraryBrowserScreen.h"


// _____________________________________________________________________________
LibraryBrowserScreen::LibraryBrowserScreen(RaspiLCD& display, Library& lib,
  Player& player) : Screen(display), _library(lib), _player(player),
                    _selectedFile(0) {
  updateCurrentFiles();
};

// _____________________________________________________________________________
void LibraryBrowserScreen::update() {
  // update the selected File
  if (_raspiLcd.buttonPressed(DOWN)) _selectedFile++;
  if (_raspiLcd.buttonPressed(UP)) _selectedFile = (_selectedFile == 0)?
                                   _currentFiles.size() - 1:_selectedFile - 1;
  // wrap around
  if (_selectedFile >= _currentFiles.size()) _selectedFile = 0;

  // if center is pressed enter directory or play file
  if (_raspiLcd.buttonPressed(CENTER)) {
    const string& cf = _currentFiles[_selectedFile];
    if (cf.substr(cf.length() - 3) == "mp3") {
      std::cout << "playing: " << cf << std::endl;
      _player.play(cf);
    } else {
      _library.cd(_selectedFile);
      updateCurrentFiles();
    }
  }

  if (_raspiLcd.buttonPressed(RIGHT)) _player.stop();

  // if left button is pressed go one directory up
  if (_raspiLcd.buttonPressed(LEFT)) {
    _library.cd(-1);
    updateCurrentFiles();
  }
}

// _____________________________________________________________________________
void LibraryBrowserScreen::draw() {
  _raspiLcd.printList(_currentFiles, _selectedFile);
}
// _____________________________________________________________________________
void LibraryBrowserScreen::updateCurrentFiles() {
  // get files from library
  const vector<string>& files = _library.getFileList();

  // clear previous version
  _currentFiles.clear();
  for (size_t i = 0; i < files.size(); i++) {
    _currentFiles.push_back(files[i]);
  }
}
