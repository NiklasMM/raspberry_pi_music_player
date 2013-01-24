// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>

#include <unistd.h>
#include <string>
#include "./RaspiLCD.h"


int main(int argc, char** argv) {
  RaspiLDC d;

  d.clear();
  
  
  d.printLine(0,0, "hello world");
  d.writeFramebuffer();
  sleep(3);
  bool v = true;
  while(true) {
    v = v?false:true;
    d.setBacklight(v);
    sleep(1);
  }
}
