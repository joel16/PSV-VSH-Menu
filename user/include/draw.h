#ifndef __DRAW_H__
#define __DRAW_H__

#include <psp2/display.h>

#define CYAN        0x00FFFF00
#define MAGENTA     0x00FF00FF
#define YELLOW      0x0000FFFF
#define RED         0x000000FF 
#define WHITE       0x00FFFFFF
#define BLACK       0x00000000
#define LIMEGREEN   0x8000FF00
#define GREEN       0x0000FF00
#define SALMON      0xC00000FF
#define SKYBLUE     0x00FF8080

#define RGB_RED         RGB(150, 1, 1) 
#define RGB_GREEN       RGB(1, 150, 1)
#define RGB_BLUE        RGB(1, 1, 150)
#define RGB_YELLOW      RGB(160, 160, 1)
#define RGB_ORANGE      RGB(255, 165, 1)
#define RGB_PURPLE      RGB(150, 1, 150)
#define RGB_PINK        RGB(255,105,180)
#define RGB_BLACK       RGB(1, 1, 1)
#define RGB_WHITE       RGB(150, 150, 150)

#define RGB(R, G, B)    (((B) << 16) | ((G) << 8) | (R))
#define RGBT(R, G, B, T) (((T) << 24) | ((B) << 16) | ((G) << 8) | (R))

#define CENTER(num) ((960 / 2) - (num*(16 / 2)))

/*
*	Sets up draw functions.
*/
SceInt drawInit(SceVoid);

/*
*	This function sets the string colour, as well as the background colour.
*/
SceVoid drawSetColour(SceInt fg_col, SceInt bg_col);

/*
*	This function draws a string onto the screen.
*/
SceInt drawString(SceInt sx, SceInt sy, const char *msg);

/*
*	This function draws a string onto the center of the screen.
*/
SceInt drawStringCenter(SceInt sy, const char *msg);

/*
*	This function draws a string onto the center of the screen with string specifier formats.
*/
SceInt drawStringfCenter(SceInt sy, const char *msg, ...);

/*
*	This function draws a string onto the screen with string specifier formats.
*/
SceInt drawStringf(SceInt sx, SceInt sy, const char *msg, ...);

/*
*	This function sets the frame buffer.
*/
SceInt drawSetFrameBuf(const SceDisplayFrameBuf *param);

/*
*	Draws a rectangle with a specified width, height and colour onto a screen.
*/
SceVoid drawRect(SceUInt32 x, SceUInt32 y, SceUInt32 w, SceUInt32 h, SceUInt32 col);

/*
*	Clears the frame buffer.
*/
SceVoid drawClear(SceVoid);

#endif
