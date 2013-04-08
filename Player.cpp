// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include <thread>
#include <string>
#include <ao/ao.h>
#include <mpg123.h>

#include "./Player.h"


// _____________________________________________________________________________
Player::Player() :
 _stopFlag(false) {

}

// _____________________________________________________________________________
void Player::play(const string& file) {
  // create thread
  _playerThread = new std::thread(&Player::playInAThread, this,  file);
  
}

// _____________________________________________________________________________
void Player::stop() {
  if (_playerThread != NULL) {
    _stopFlag = true;
    _playerThread->join();
    delete _playerThread;
    _stopFlag = false;
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
}