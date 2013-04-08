// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_PLAYER_H_
#define JUKEBERRY_PLAYER_H_

#include <string>
#include <atomic>
#include <thread>

using std::string;

class Player {
 public:
  // default constructor
  Player();

  // play the given mp3 file
  void play(const string& file);

  // stop playing a song
  void stop();
 private:
  // this method will be run as a thread and do the actual playback
  void playInAThread(const string& file);

  // if this flag is set to true, the player thread will stop playing
  std::atomic_bool _stopFlag;

  std::thread* _playerThread;
};

#endif  // JUKEBERRY_PLAYER_H_
