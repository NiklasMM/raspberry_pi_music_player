// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)


#include <chrono>
#include "./LibraryBrowserScreen.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;

// _____________________________________________________________________________
LibraryBrowserScreen::LibraryBrowserScreen(RaspiLCD& display, JukeBerry* jb,
                                          Library& lib, Player& player) :
  Screen(display, jb), _library(lib), _player(player),
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

  // if the currently selected file is longer than the display, scroll
  if (_currentFiles[_selectedFile].length() > _raspiLcd.getMaxCharactersPerLine()) {
    auto now = high_resolution_clock::now();
    auto used = now - _lastScrollTime;
    double timePassed = duration_cast<milliseconds>(used).count();
    if (timePassed > 400) {
      // remove the path
      const string& currentFile = _currentFiles[_selectedFile];
      size_t lastFileSeparator = currentFile.find_last_of("/");
      if (lastFileSeparator == string::npos) lastFileSeparator = 0;
      string displayedString = currentFile.substr(lastFileSeparator + 1);
            
      // the number of scroll positions is equal to the number of characters that
      // the string is longer than the display
      size_t scrollPositions = displayedString.length()
                               - _raspiLcd.getMaxCharactersPerLine();
      _lastScrollPos++;
      if (_lastScrollPos > scrollPositions) _lastScrollPos = 0;
      _lastScrollTime = now;

      // actually scroll the string
      _displayedFiles[_selectedFile] = displayedString.substr(
                          _lastScrollPos, _raspiLcd.getMaxCharactersPerLine());
    }
  }
}

// _____________________________________________________________________________
void LibraryBrowserScreen::draw() {
  _raspiLcd.printList(0, 0, _displayedFiles, _selectedFile);
}
// _____________________________________________________________________________
void LibraryBrowserScreen::updateCurrentFiles() {
  // get files from library
  const vector<string>& files = _library.getFileList();

  // clear previous version
  _currentFiles.clear();
  _displayedFiles.clear();
  for (size_t i = 0; i < files.size(); i++) {
    _currentFiles.push_back(files[i]);
    // process the displayed string:
    // first remove the path
    size_t lastFileSeparator = files[i].find_last_of("/");
    if (lastFileSeparator == string::npos) lastFileSeparator = 0;
    string displayedString = files[i].substr(lastFileSeparator + 1);
    _displayedFiles.push_back(
      displayedString.substr(0, _raspiLcd.getMaxCharactersPerLine()));
    std::cout << _displayedFiles.back() << std::endl;
  }

  // reset last scroll time
  _lastScrollTime = high_resolution_clock::now();
  _lastScrollPos = 0;
}
