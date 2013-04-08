// Author: Niklas Meinzer <meinzer.niklas@gmail.com>
// This code is open-source under the terms of the GPLv3 (see LICENSE file)

#ifndef JUKEBERRY_CFUNCTIONS_H_
#define JUKEBERRY_CFUNCTIONS_H_

// these are the C functions from lcd.h and raspilcd.h which are used in the
// C++ code of the project
extern "C" {
  int RaspiLcdHwInit(void);
  void SetBacklight(uint8 light);
  void UpdateButtons(void);
  void LCD_Init(void);
  void LCD_ClearScreen(void);
  void LCD_SetPenColor(uint8 c);
  void LCD_SetFont(uint8 f);
  void LCD_SetContrast(uint8 contrast);
  void LCD_PrintXY(uint8 x,uint8 y,char *s);
  void LCD_WriteFramebuffer(void);
  void getButtonStates(int* up, int* down, int* left, int* right, int* center);
}


#endif  // JUKEBERRY_CFUNCTIONS_H_
