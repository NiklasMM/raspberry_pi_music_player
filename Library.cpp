// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "./Library.h"

using namespace boost::filesystem;

using std::vector;
using std::string;
using std::cout;

// _____________________________________________________________________________
Library::Library(const string& basePath){
  path baseDir(basePath);
  try {
    if (exists(baseDir)) {
      if (!is_directory(baseDir)) {
        cout << baseDir << " is not directory!\n";
      }
      _basepath = basePath;
    }
    else
      cout << baseDir << " does not exist\n";
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  _currentPath = baseDir;
};


// _____________________________________________________________________________
vector<string> Library::getFileList() const {
  vector<string> result;

  for(directory_iterator it(_currentPath); it != directory_iterator(); ++it) {
    result.push_back((*it).path().native());
  }
  return result;
}
