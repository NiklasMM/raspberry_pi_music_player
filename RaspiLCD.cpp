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

extern "C" {
  int RaspiLcdHwInit(void);
  void SetBacklight(uint8 light);
  void LCD_Init(void);
  void LCD_ClearScreen(void);
  void LCD_SetPenColor(uint8 c);
  void LCD_SetFont(uint8 f);
  void LCD_SetContrast(uint8 contrast);
  void LCD_PrintXY(uint8 x,uint8 y,char *s);
  void LCD_WriteFramebuffer(void);
}

using std::ifstream;
using std::string;

// _____________________________________________________________________________
RaspiLCD::RaspiLCD()
 : _backlight(false) {

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
void RaspiLCD::printList(const vector<string>& lines) {
  LCD_ClearScreen();
	LCD_SetFont(0);
  char* str = const_cast<char*>(lines[0].c_str());
	LCD_PrintXY(0,0,str);
  LCD_WriteFramebuffer();
}
