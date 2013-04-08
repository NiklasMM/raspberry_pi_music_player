// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include <unistd.h>
#include <string>
#include <vector>
#include "./JukeBerry.h"


using std::vector;
using std::string;

int main(int argc, char** argv) {
  JukeBerry jb(argv[1]);

  jb.start();
}
