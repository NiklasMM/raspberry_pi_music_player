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
  // create a path from the given string
  path baseDir = absolute(basePath);
  // canonicalize
  baseDir = boost::filesystem::canonical(baseDir);

  try {
    if (exists(baseDir)) {
      if (!is_directory(baseDir)) {
        cout << baseDir << " is not directory!\n";
      }
      _basepath = baseDir.native();
    }
    else
      cout << baseDir << " does not exist\n";

    // cd into the root
    _currentDirFiles.push_back(basePath);
    cd(0);
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  _currentPath = baseDir;
};


// _____________________________________________________________________________
const vector<string>& Library::getFileList() const {
  return _currentDirFiles;
}

// _____________________________________________________________________________
int Library::cd(int i) {

  if (i >= static_cast<int>(_currentDirFiles.size())) return 1;

  // if i >= 0 switch into deeper directory
  if (i >= 0) {
    // create a path object for the new directory
    path newDir(_currentDirFiles[i]);
    newDir = boost::filesystem::canonical(newDir);

    // return if it is not a directory
    if (!is_directory(newDir)) return 1;

    // push current path to parents
    _parents.push_back(_currentPath);

    // actually cd into newDir
    cd(newDir);
  } else { // if i < 0 switch into parent directory
    path parent = _parents.back();
    // if the parent is "" this is the root directory
    if (parent == "") return -1;

    std::cout << "cahnging into " << parent.native() << std::endl;

    // pop the last element
    _parents.pop_back();
    // actually cd into parent
    cd(parent);
    return 0;
    }
}
// _____________________________________________________________________________
void Library::cd(const path& p) {
  
  // return if it is not a directory
  if (!is_directory(p)) return;

  // reset current path
  _currentPath = p;

  // update file list
  _currentDirFiles.clear();

  for(directory_iterator it(_currentPath); it != directory_iterator(); ++it) {
    path newPath = boost::filesystem::canonical((*it).path());
    _currentDirFiles.push_back(newPath.native());
  }
}
