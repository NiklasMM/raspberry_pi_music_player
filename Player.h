// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_PLAYER_H_
#define JUKEBERRY_PLAYER_H_

#include <string>
#include <atomic>
#include <thread>
#include <memory>
#include <queue>


using std::string;
using std::vector;
using std::queue;

class Player {
 public:
  // default constructor
  Player();

  // play the given mp3 file
  void play(const string& file);

  // enqueues a file in the playlist
  void enqueue(const string& file);

  // called in every update of the Jukeberry. Checks if a new song needs to
  // be played
  void update();

  // stop playing a song
  void stop();

  // returns a reference to the playqueue
  const queue<string>& getPlayQueue() const;
 private:
  // this method will be run as a thread and do the actual playback
  void playInAThread(const string& file);

  // if this flag is set to true, the player thread will stop playing
  std::atomic_bool _stopFlag;

  std::atomic_bool _playing;

  // a pointer to the player thread, i.e. the actuall decoding and playing
  // of the mp3 happens here
  std::unique_ptr<std::thread> _playerThread;

  // a list of files to be played in the future
  queue<string> _playlist;
};

#endif  // JUKEBERRY_PLAYER_H_
