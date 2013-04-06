// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_LIBRARY_H_
#define JUKEBERRY_LIBRARY_H_

#include <boost/filesystem.hpp>
#include <string>
#include <vector>

using std::vector;
using std::string;

// A class that manages a music library
class Library {
 public:
  Library(const string& basePath);

  // returns the files in the current directory
  vector<string> getFileList() const;

 private:
  // the base path of the library
  string _basepath;
  // the current path
  boost::filesystem::path _currentPath;
};

#endif  // JUKEBERRY_LIBRARY_H_
