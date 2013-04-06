// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include <unistd.h>
#include <string>
#include <vector>
#include "./RaspiLCD.h"
#include "./Library.h"


using std::vector;
using std::string;

int main(int argc, char** argv) {
  Library l(argv[1]);

  int dirChoice = 0;
  do {
    const vector<string>& files = l.getFileList();

    for (size_t i = 0; i < files.size(); i++) {
      string file = files[i];
      std::cout << files[i] << std::endl;
      //~ if (file.substr(file.length() - 3) == "mp3")
        //~ system(("mpg123 \"" + file + "\"").c_str());
    }

    std::string line;
    std::getline(std::cin, line);

    dirChoice = atoi(line.c_str());

    std::cout << "dir choice: " << dirChoice << std::endl;

    l.cd(dirChoice);
  } while (true);

}
