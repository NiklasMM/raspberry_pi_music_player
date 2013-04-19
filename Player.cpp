// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include <thread>
#include <string>
#include <ao/ao.h>
#include <mpg123.h>
#include <iostream>

#include "./Player.h"


// _____________________________________________________________________________
Player::Player() :
 _stopFlag(false), _playing(false), _playerThread(nullptr) {

}

// _____________________________________________________________________________
void Player::play(const string& file) {
  if (_playerThread != nullptr) stop();
  _playing = true;
  // create thread
  std::cout << "playing: " << file << std::endl;
  _playerThread.reset(new std::thread(&Player::playInAThread, this,  file));
}

// _____________________________________________________________________________
void Player::stop() {
  if (_playerThread != nullptr) {
    // set the stop flag which asks the player thread to stop playback and
    // terminate
    _stopFlag = true;
    // wait for the thread to actually terminate
    _playerThread->join();
    // destroy the thread and set the thread pointer to NULL
    _playerThread.reset(nullptr);
    // reset the stop Flag
    _stopFlag = false;
  }
}

// _____________________________________________________________________________
void Player::enqueue(const string& file) {
  _playlist.push(file);
}

// _____________________________________________________________________________
void Player::update() {
  // check if a song is being played
  if (! _playing.load()) {
    // check if songs are left in the playlist
    if(_playlist.size() != 0) {
      string nextSong = _playlist.front();
      play(nextSong);
      _playlist.pop();
    }
  }
}

// _____________________________________________________________________________
void Player::playInAThread(const string& file) {
  mpg123_handle *mh;
  unsigned char *buffer;
  size_t buffer_size;
  size_t done;
  int err;

  int driver;
  ao_device *dev;

  ao_sample_format format;
  int channels, encoding;
  long rate;

  /* initializations */
  ao_initialize();
  driver = ao_default_driver_id();
  mpg123_init();
  mh = mpg123_new(NULL, &err);
  buffer_size = mpg123_outblock(mh);
  buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

  /* open the file and get the decoding format */
  mpg123_open(mh, file.c_str());
  mpg123_getformat(mh, &rate, &channels, &encoding);

  /* set the output format and open the output device */
  format.bits = mpg123_encsize(encoding) * 8;
  format.rate = rate;
  format.channels = channels;
  format.byte_format = AO_FMT_NATIVE;
  format.matrix = 0;
  dev = ao_open_live(driver, &format, NULL);

  /* decode and play */
  while (!_stopFlag.load() && mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
      ao_play(dev, reinterpret_cast<char*>(buffer), done);

  /* clean up */
  free(buffer);
  ao_close(dev);
  mpg123_close(mh);
  mpg123_delete(mh);
  mpg123_exit();
  ao_shutdown();

  _playing = false;
}
// _____________________________________________________________________________
const queue<string>& Player::getPlayQueue() const {
  return _playlist;
}
