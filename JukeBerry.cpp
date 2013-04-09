// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)


#include <string>
#include <thread>

#include "./bcm2835.h"
#include "./JukeBerry.h"
#include "./RaspiLCD.h"
#include "./Library.h"

// _____________________________________________________________________________
JukeBerry::JukeBerry(const string& path) :
  _library(Library(path)), _selectedFile(0){
  
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

  // update the selected File
  if (_display.buttonPressed(DOWN)) _selectedFile++;
  if (_display.buttonPressed(UP)) _selectedFile--;

  // if center is pressed enter directory
  if (_display.buttonPressed(CENTER)) {
    const vector<string>& files = _library.getFileList();
    const string& cf = files[_selectedFile];
    if (cf.substr(cf.length() - 3) == "mp3") {
      _player.play(cf);
    } else {
      _library.cd(_selectedFile);
    }
  }

  if (_display.buttonPressed(RIGHT)) _player.stop();

  // if left button is pressed go one directory up
  if (_display.buttonPressed(LEFT)) _library.cd(-1);
}

// _____________________________________________________________________________
void JukeBerry::draw() {
      const vector<string>& files = _library.getFileList();

    //~ for (size_t i = 0; i < files.size(); i++) {
      //~ string file = files[i];
      //~ std::cout << files[i] << std::endl;
    //~ }

    _display.printList(files, _selectedFile);
}
