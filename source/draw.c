#include <libk/stdarg.h>
#include <libk/stdio.h>
#include <libk/string.h>

#include <psp2/types.h>
#include <psp2/display.h>

#include "draw.h"

#define ALPHA_BLEND 1

extern SceUChar8 msx[];

static SceInt pwidth, pheight, bufferwidth, pixelformat;
static SceSize *vram32;

static SceUInt32 fcolor = 0x00ffffff;
static SceUInt32 bcolor = 0xff000000;

#if ALPHA_BLEND
static SceUInt32 adjust_alpha(SceUInt32 col)
{
	SceUInt32 alpha = col>>24;
	SceUInt8 mul;
	SceUInt32 c1,c2;

	if (alpha == 0)
		return col;
	if (alpha == 0xff)
		return col;

	c1 = col & 0x00ff00ff;
	c2 = col & 0x0000ff00;
	mul = (SceUInt8)(255-alpha);
	c1 = ((c1*mul)>>8)&0x00ff00ff;
	c2 = ((c2*mul)>>8)&0x0000ff00;
	return (alpha<<24)|c1|c2;
}
#endif

/*
*	Sets up draw functions.
*/
SceInt drawInit(SceVoid)
{
	SceDisplayFrameBuf param;
	param.size = sizeof(SceDisplayFrameBuf);
	sceDisplayGetFrameBuf(&param, SCE_DISPLAY_SETBUF_IMMEDIATE);

	pwidth = param.width;
	pheight = param.height;
	vram32 = param.base;
	bufferwidth = param.pitch;
	pixelformat = param.pixelformat;

	if ((bufferwidth == 0) || (pixelformat != 0)) 
		return -1;

	fcolor = 0x00ffffff;
	bcolor = 0xff000000;

	return 0;
}

/*
*	This function sets the string colour, as well as the background colour.
*/
SceVoid drawSetColour(SceInt fg_col, SceInt bg_col)
{
	fcolor = fg_col;
	bcolor = bg_col;
}

/*
*	This function draws a string onto the screen.
*/
SceInt drawString(SceInt sx, SceInt sy, const char *msg)
{
	SceInt x, y, p;
	SceInt offset;
	char code;
	unsigned char font;
	SceUInt32 fg_col, bg_col;

#if ALPHA_BLEND
	SceUInt32 col,c1,c2;
	SceUInt32 alpha;

	fg_col = adjust_alpha(fcolor);
	bg_col = adjust_alpha(bcolor);
#else
	fg_col = fcolor;
	bg_col = bcolor;
#endif

//Kprintf("MODE %d WIDTH %d\n",pixelformat,bufferwidth);
	if ((bufferwidth == 0) || (pixelformat != 0)) 
		return -1;

	for(x = 0; msg[x] && x < (pwidth / 16); x++)
	{
		code = msg[x] & 0x7f; // 7bit ANK
		for(y = 0; y < 8; y++)
		{
			offset = (sy +(y * 2)) * bufferwidth + sx + x * 16;
			font = y >= 7 ? 0x00 : msx[ code * 8 + y ];
			for(p = 0; p < 8; p++)
			{
#if ALPHA_BLEND
				col = (font & 0x80) ? fg_col : bg_col;
				alpha = col>>24;
				if (alpha == 0)
				{
					vram32[offset] = col;
					vram32[offset + 1] = col;
					vram32[offset + bufferwidth] = col;
					vram32[offset + bufferwidth + 1] = col;
				}
				else if (alpha != 0xff)
				{
					c2 = vram32[offset];
					c1 = c2 & 0x00ff00ff;
					c2 = c2 & 0x0000ff00;
					c1 = ((c1*alpha)>>8)&0x00ff00ff;
					c2 = ((c2*alpha)>>8)&0x0000ff00;
					SceUInt32 color = (col&0xffffff) + c1 + c2;
					vram32[offset] = color;
					vram32[offset + 1] = color;
					vram32[offset + bufferwidth] = color;
					vram32[offset + bufferwidth + 1] = color;
				}
#else
				SceUInt32 color = (font & 0x80) ? fg_col : bg_col;
				vram32[offset] = color;
				vram32[offset + 1] = color;
				vram32[offset + bufferwidth] = color;
				vram32[offset + bufferwidth + 1] = color;
#endif
				font <<= 1;
				offset += 2;
			}
		}
	}
	return x;
}

/*
*	This function draws a string onto the center of the screen.
*/
SceInt drawStringCenter(SceInt sy, const char *msg)
{
	SceInt sx = (960 / 2) - (strlen(msg) * (16 / 2));
	return drawString(sx, sy, msg);
}

/*
*	This function draws a string onto the center of the screen with string specifier formats.
*/
SceInt drawStringfCenter(SceInt sy, const char *msg, ...)
{
	va_list list;
	char string[512];
	
	va_start(list, msg);
	vsnprintf(string, 512, msg, list);
	va_end(list);
	
	SceInt sx = (960 / 2) - (strlen(string) * (16 / 2));
	return drawString(sx, sy, string);
}

/*
*	This function draws a string onto the screen with string specifier formats.
*/
SceInt drawStringf(SceInt sx, SceInt sy, const char *msg, ...)
{
	va_list list;
	char string[512];

	va_start(list, msg);
	vsnprintf(string, 512, msg, list);
	va_end(list);

	return drawString(sx, sy, string);
}

/*
*	This function sets the frame buffer.
*/
SceInt drawSetFrameBuf(const SceDisplayFrameBuf *param)
{	
	pwidth = param->width;
	pheight = param->height;
	vram32 = param->base;
	bufferwidth = param->pitch;
	pixelformat = param->pixelformat;

	if ((bufferwidth == 0) || (pixelformat != 0)) 
		return -1;

	fcolor = 0x00ffffff;
	bcolor = 0xff000000;

	return 0;
}

/*
*	Draws a rectangle with a specified width, height and colour onto a screen.
*/
SceVoid drawRect(SceUInt32 x, SceUInt32 y, SceUInt32 w, SceUInt32 h, SceUInt32 col)
{
	SceUInt32 c1,c2;
	SceUInt32 in_col = adjust_alpha(col);
	SceUInt32 alpha = in_col>>24;
	
	for (SceInt i = 0; i < h; i++) 
	{
		for (SceInt j = 0; j < w; j++) 
		{
			c2 = vram32[(x + j) + (y + i) * bufferwidth];
			c1 = c2 & 0x00ff00ff;
			c2 = c2 & 0x0000ff00;
			c1 = ((c1*alpha)>>8)&0x00ff00ff;
			c2 = ((c2*alpha)>>8)&0x0000ff00;
			SceUInt32 color = (in_col&0xffffff) + c1 + c2;
			((SceUInt32 *)vram32)[(x + j) + (y + i) * bufferwidth] = color;
		}
	}
}
