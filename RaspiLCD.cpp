// Copyright 2013
// Author: Niklas Meinzer <meinzer.niklas@gmail.com>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include "./RaspiLCD.h"
#include "./bcm2835.h"

using std::ifstream;
using std::string;

// _____________________________________________________________________________
RaspiLDC::RaspiLDC()
 : _backlight(false) {
  // resize the vector to 256
  _pixels.resize(256);
  // resize the button pin vector
  _buttonPins.resize(5);

  // resize framebuffer
  _framebuffer.resize(LCD_WIDTH);
  for (size_t i = 0; i < _framebuffer.size(); i++)
    _framebuffer[i].resize(LCD_HEIGHT/8);

  initialize();

  _font = font_terminal_6x8;
}

// _____________________________________________________________________________
int RaspiLDC::initialize()
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
void RaspiLDC::setBacklight(bool value) {
  _backlight = value;

  if (_backlight) {
    // set the backlight controlling pin
    bcm2835_gpio_set(PIN_LCD_BACKLIGHT);
  } else {
    // clear the backlight controlling pin
    bcm2835_gpio_clr(PIN_LCD_BACKLIGHT);
  }
}

// _____________________________________________________________________________
size_t RaspiLDC::GetRaspberryHwRevision()
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
void RaspiLDC::clear() {
  // iterate over entries in the framebuffer and zero them
  for (size_t i = 0; i < _framebuffer.size(); i++)
    for (size_t j = 0; j < _framebuffer.size(); j++)
      _framebuffer[i][j] = 0;
}

// _____________________________________________________________________________
void RaspiLDC::putPixel(size_t x, size_t y, size_t color) {
	if((x < LCD_WIDTH) && (y < LCD_HEIGHT))
	{
		if(color)	_framebuffer[x][y>>3] |=   (1<<(y & 7));
			else	_framebuffer[x][y>>3] &=  ~(1<<(y & 7));
	}
}

// _____________________________________________________________________________
void RaspiLDC::printLine(size_t x0, size_t y0, char *s)
{
	uint8	ix,iy,y;
	const uint8 *pt;
	uint8 	d;
	char c;
	uint8 char_width,char_height,char_size;
	uint8 i_char_height;

  const uint8 *font = _font;
  
	char_size =  *font++;
	char_width  = *font++;
	char_height = *font++;
	
	while(*s)
	{
		c = 0;
		if(*s > 31) c = (*s) - 32;
		pt = &font[(uint16)c * char_size];		
		i_char_height = char_height;
		
		y = y0;
				
		while(i_char_height)
		{
			for(ix=0;ix<char_width;ix++)   
			{
				d = *pt++;
				for(iy=0;iy<8;iy++)	// je ein Byte vertikal ausgeben
				{
					if(d & (1<<iy))	putPixel(x0+ix,y+iy,1);	
						else			putPixel(x0+ix,y+iy,0);	
				}
			}
			i_char_height = (i_char_height >= 8) ? i_char_height - 8 : 0; 
			y+=8;		// nächste "Page"
		}	

		x0+=char_width;	
		s++;		// nächstes Zeichen
	}
}

// _____________________________________________________________________________
void RaspiLDC::lcd_write_data(uint8 d) {
	bcm2835_gpio_clr(PIN_LCD_CS);
  bcm2835_gpio_set(PIN_LCD_RS);			// Data Mode
	spiPutc(d);
	//~ LCD_SPI_WAIT_BUSY;
	bcm2835_gpio_set(PIN_LCD_CS);
}

// _____________________________________________________________________________
void RaspiLDC::lcd_write_cmd(uint8 d)
{
	bcm2835_gpio_clr(PIN_LCD_CS);
	bcm2835_gpio_clr(PIN_LCD_RS);			// Command Mode
	spiPutc(d);
	//~ LCD_SPI_WAIT_BUSY;
	bcm2835_gpio_set(PIN_LCD_CS);
}

// _____________________________________________________________________________
void RaspiLDC::spiPutc(unsigned char d)
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
void RaspiLDC::writeFramebuffer() {
	uint8	 x;
	lcd_set_xy(0,0);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][0]);
	lcd_set_xy(0,1);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][1]);
	lcd_set_xy(0,2);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][2]);
	lcd_set_xy(0,3);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][3]);
	lcd_set_xy(0,4);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][4]);
	lcd_set_xy(0,5);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][5]);
	lcd_set_xy(0,6);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][6]);
	lcd_set_xy(0,7);	for(x=0;x<LCD_WIDTH;x++)  lcd_write_data(_framebuffer[x][7]);
}

// _____________________________________________________________________________
void RaspiLDC::lcd_set_xy(uint8 x,uint8 ypage)
{
	x += LCD_X_OFFSET;
	lcd_write_cmd(0x00 + (x & 0x0F));
	lcd_write_cmd(0x10 + ((x>>4) & 0x0F));
	lcd_write_cmd(0xB0 + (ypage & 0x07));
}
