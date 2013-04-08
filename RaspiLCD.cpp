// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include "./RaspiLCD.h"
#include "./bcm2835.h"

#include "./cFunctions.h"

using std::ifstream;
using std::string;

// _____________________________________________________________________________
RaspiLCD::RaspiLCD()
 : _backlight(false), _buttonUp(0), _buttonDown(0), _buttonLeft(0), _buttonRight(0),
   _buttonCenter(0) {

   // initialize the display
  if(!RaspiLcdHwInit()){
    std::cerr << "RaspiLcdHwInit() failed!\r\n";
  }

  LCD_Init();			// Init Display
	setBacklight(true);	// Turn Backlight on
}

// _____________________________________________________________________________
void RaspiLCD::setBacklight(bool value) {
  _backlight = value;

  if (_backlight) {
    SetBacklight(1);
  } else {
    SetBacklight(0);
  }
}

// _____________________________________________________________________________
size_t RaspiLCD::GetRaspberryHwRevision()
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

// _____________________________________________________________________________
void RaspiLCD::clear() {
  // iterate over entries in the framebuffer and zero them
  for (size_t i = 0; i < _framebuffer.size(); i++)
    for (size_t j = 0; j < _framebuffer.size(); j++)
      _framebuffer[i][j] = 0;
}

// _____________________________________________________________________________
void RaspiLCD::putPixel(size_t x, size_t y, size_t color) {
	if((x < LCD_WIDTH) && (y < LCD_HEIGHT))
	{
		if(color)	_framebuffer[x][y>>3] |=   (1<<(y & 7));
			else	_framebuffer[x][y>>3] &=  ~(1<<(y & 7));
	}
}


// _____________________________________________________________________________
void RaspiLCD::lcd_write_data(uint8 d) {
	bcm2835_gpio_clr(PIN_LCD_CS);
  bcm2835_gpio_set(PIN_LCD_RS);			// Data Mode
	spiPutc(d);
	//~ LCD_SPI_WAIT_BUSY;
	bcm2835_gpio_set(PIN_LCD_CS);
}

// _____________________________________________________________________________
void RaspiLCD::lcd_write_cmd(uint8 d)
{
	bcm2835_gpio_clr(PIN_LCD_CS);
	bcm2835_gpio_clr(PIN_LCD_RS);			// Command Mode
	spiPutc(d);
	//~ LCD_SPI_WAIT_BUSY;
	bcm2835_gpio_set(PIN_LCD_CS);
}

// _____________________________________________________________________________
void RaspiLCD::spiPutc(unsigned char d)
{
	int i,n;
	
	for(i=0;i<8;i++)
	{
		if(d & 0x80)	bcm2835_gpio_set(PIN_LCD_MOSI);		// MOSI = 1
			else		bcm2835_gpio_clr(PIN_LCD_MOSI);		// MOSI = 0
		d <<= 1;
		
		for(n=0;n<4;n++) bcm2835_gpio_clr(PIN_LCD_SCLK); 	// CLK = 0
		for(n=0;n<4;n++) bcm2835_gpio_set(PIN_LCD_SCLK);	// CLK = 1
	}
}

// _____________________________________________________________________________
void RaspiLCD::lcd_set_xy(uint8 x,uint8 ypage)
{
	x += LCD_X_OFFSET;
	lcd_write_cmd(0x00 + (x & 0x0F));
	lcd_write_cmd(0x10 + ((x>>4) & 0x0F));
	lcd_write_cmd(0xB0 + (ypage & 0x07));
}

// _____________________________________________________________________________
void RaspiLCD::printList(const vector<string>& lines, int selected) {
  LCD_ClearScreen();
	LCD_SetFont(0);

  // iterate over all lines
  for (size_t i = 0; i < lines.size(); i++) {
    // stop if more than MAX_LINES_DISPLAYED lines are present
    if (i >= MAX_LINES_DISPLAYED) break;

    const string& line = lines[i];
    // only display the filename, that is after the last "/"
    size_t lastFileSeparator = line.find_last_of("/");
    if (lastFileSeparator == string::npos) lastFileSeparator = 0;
    string displayedString = line.substr(lastFileSeparator + 1);

    // to avoid overflow, crop the string to MAX_CHARS_PER_LINE characters
    size_t charsDisplayed = (MAX_CHARS_PER_LINE > displayedString.length())?displayedString.length():MAX_CHARS_PER_LINE;
    displayedString = displayedString.substr(0,charsDisplayed);

    // print a ">" in front of the selected file
    if (selected == static_cast<int>(i)) {
      displayedString = ">" + displayedString;
    } else {
      displayedString = " " + displayedString;
    }

    LCD_PrintXY(0,8*i-1 + 2,const_cast<char*>(displayedString.c_str()));
  }
  LCD_WriteFramebuffer();
}

// _____________________________________________________________________________
void RaspiLCD::update() {
  // update buttons
  UpdateButtons();
  // reset button states
  _buttonUp = 0; _buttonDown = 0; _buttonLeft = 0; _buttonRight = 0; _buttonCenter = 0;

  getButtonStates(&_buttonUp, &_buttonDown, &_buttonLeft, &_buttonRight, &_buttonCenter);
}


// _____________________________________________________________________________
bool RaspiLCD::buttonPressed(RaspiLcdButtons button) const {
  switch (button) {
    case UP:
      return (_buttonUp == 1);
      break;
    case DOWN:
      return (_buttonDown == 1);
      break;
    case LEFT:
      return (_buttonLeft == 1);
      break;
    case RIGHT:
      return (_buttonRight == 1);
    case CENTER:
      return (_buttonCenter == 1);
      break;
    default:
      return false;
  }
}
