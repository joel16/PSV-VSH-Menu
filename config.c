#include <libk/stdio.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "utils.h"

const char * configFile =
	"display_battery = %d\n"
	"bar_colour = %d\n";

SceInt saveConfig(SceBool batteryPercent, int colour)
{
	SceInt ret = 0;
	
	char buf[512];
	int len = snprintf(buf, 512, configFile, batteryPercent, colour);
	
	if (R_FAILED(ret = writeFile("ux0:/data/vsh/config.cfg", buf, len)))
		return ret;
	
	return 0;
}

SceInt loadConfig(SceVoid)
{
	SceInt ret = 0;
	
	if (!(fileExists("ux0:/data/vsh/config.cfg")))
	{
		// set these to the following by default:
		batteryPercent = 0;
		colour = 0;
		return saveConfig(batteryPercent, colour);
	}
	
	char buf[512];
	
	if (R_FAILED(ret = readFile("ux0:/data/vsh/config.cfg", buf, 512)))
		return ret;
	
	sscanf(buf, configFile, &batteryPercent, &colour);
	
	return 0;
}

int getColour()
{
	int col = 0;
	
	switch (colour)
	{
		case 0:
			col = RGB_RED;
			break;
		case 1:
			col = RGB_GREEN;
			break;
		case 2:
			col = RGB_BLUE;
			break;
		case 3:
			col = RGB_YELLOW;
			break;
		case 4:
			col = RGB_ORANGE;
			break;
		case 5:
			col = RGB_PURPLE;
			break;
		case 6:
			col = RGB_PINK;
			break;
		case 7:
			col = RGB_BLACK;
			break;
		case 8:
			col = RGB_WHITE;
			break;
	}
	
	return col;
}