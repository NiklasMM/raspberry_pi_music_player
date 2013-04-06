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

using boost::filesystem::path;

// A class that manages a music library
class Library {
 public:
  Library(const string& basePath);

  // returns the files in the current directory
  const vector<string>& getFileList() const;

  // changes into the i-th file of the current directory if it is a directory
  // or to the parent directory if i == -1
  void cd(int i);

 private:
  // the base path of the library
  string _basepath;
  // the current path
  path _currentPath;

  // the content of the current directory
  vector<string> _currentDirFiles;

  // changes into a directory specified by p
  void cd(const path& p);

  // a stack of directories containing the parents of the current directory
  // until the base
  vector<path> _parents;
};

#endif  // JUKEBERRY_LIBRARY_H_
