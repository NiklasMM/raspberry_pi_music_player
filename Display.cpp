// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include "./Display.h"
#include "./bcm2835.h"

using std::ifstream;
using std::string;

// _____________________________________________________________________________
Display::Display()
 : _backlight(false) {
  // resize the vector to 256
  _pixels.resize(256);
  // resize the button pin vector
  _buttonPins.resize(5);

  initialize();
}

// _____________________________________________________________________________
int Display::initialize()
{
	int HwRev;
	
	HwRev = GetRaspberryHwRevision();

	if (!bcm2835_init()) return 0;
	
	// Buttons 	
	_buttonPins[0] = 	17;
	_buttonPins[1] = (HwRev < 2) ? 21 : 27;
	_buttonPins[2] = 22;
	_buttonPins[3] = 23;
	_buttonPins[4] = 24;
	
	bcm2835_gpio_fsel(_buttonPins[0],BCM2835_GPIO_FSEL_INPT)	;	// Set GPIO Pin to Input 
	bcm2835_gpio_fsel(_buttonPins[1],BCM2835_GPIO_FSEL_INPT)	;	// Set GPIO Pin to Input 
	bcm2835_gpio_fsel(_buttonPins[2],BCM2835_GPIO_FSEL_INPT)	;	// Set GPIO Pin to Input 
	bcm2835_gpio_fsel(_buttonPins[3],BCM2835_GPIO_FSEL_INPT)	;	// Set GPIO Pin to Input 
	bcm2835_gpio_fsel(_buttonPins[4],BCM2835_GPIO_FSEL_INPT)	;	// Set GPIO Pin to Input 

	bcm2835_gpio_set_pud(_buttonPins[0],BCM2835_GPIO_PUD_UP); 	// Enable Pullup
	bcm2835_gpio_set_pud(_buttonPins[1],BCM2835_GPIO_PUD_UP); 	// Enable Pullup
	bcm2835_gpio_set_pud(_buttonPins[2],BCM2835_GPIO_PUD_UP); 	// Enable Pullup
	bcm2835_gpio_set_pud(_buttonPins[3],BCM2835_GPIO_PUD_UP); 	// Enable Pullup
	bcm2835_gpio_set_pud(_buttonPins[4],BCM2835_GPIO_PUD_UP); 	// Enable Pullup	
	
	// LCD Display
	bcm2835_gpio_fsel(PIN_LCD_MOSI,     BCM2835_GPIO_FSEL_OUTP);	// GPIO10 Output: MOSI
	bcm2835_gpio_fsel(PIN_LCD_SCLK,     BCM2835_GPIO_FSEL_OUTP);	// GPIO11 Output: SCLK
	bcm2835_gpio_fsel(PIN_LCD_RST,      BCM2835_GPIO_FSEL_OUTP);	// GPIO25 Output: RST
	bcm2835_gpio_fsel(PIN_LCD_CS ,      BCM2835_GPIO_FSEL_OUTP);	// GPIO8  Output: CS
	bcm2835_gpio_fsel(PIN_LCD_RS,       BCM2835_GPIO_FSEL_OUTP);	// GPIO7  Output: RS
	bcm2835_gpio_fsel(PIN_LCD_BACKLIGHT,BCM2835_GPIO_FSEL_OUTP);	// GPIO18 Output: Backlight
	
	_button = _buttonMem = _buttonPressed = 0;
	
	return 1;
}

// _____________________________________________________________________________
void Display::setBacklight(bool value) {
  _backlight = value;

  if (_backlight) {
    // set the backlight controlling pin
    bcm2835_gpio_set(PIN_LCD_BACKLIGHT);
  } else {
    // clear the backlight controlling pin
    bcm2835_gpio_clr(PIN_LCD_BACKLIGHT);
  }
}

size_t Display::GetRaspberryHwRevision()
{	
	ifstream file("/proc/cpuinfo");
	string line;

  // iterate over lines of the file
	while(true)
	{
    getline(file, line);
    if (file.eof()) break;

    // find the line containing "Revision"
    if (line.find("Revision") != string::npos) {
      string rev = line.substr(line.find(':') + 1);
      return static_cast<size_t>(atoi(rev.c_str()));
    }
  }
	return 0;
}
