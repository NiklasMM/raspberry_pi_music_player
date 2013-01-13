// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>

#include <unistd.h>
#include "./Display.h"


int main(int argc, char** argv) {
  Display d;

  bool v = true;
  while(true) {
    v = v?false:true;
    d.setBacklight(v);
    sleep(1);
  }
}
