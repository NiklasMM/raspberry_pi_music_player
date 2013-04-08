// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)


#include <string>

#include "./bcm2835.h"
#include "./JukeBerry.h"
#include "./RaspiLCD.h"
#include "./Library.h"


// _____________________________________________________________________________
JukeBerry::JukeBerry(const string& path) :
  _library(Library(path)){
  
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
}

// _____________________________________________________________________________
void JukeBerry::draw() {
}
