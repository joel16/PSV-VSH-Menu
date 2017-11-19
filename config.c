#include <libk/stdio.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "utils.h"

const char * menuConfig =
	"display_battery = %d\n"
	"bar_colour = %d\n";

const char * clockConfig =
	"CPU_clock = %d\n"
	"GPU_clock = %d\n";

SceInt saveMenuConfig(SceBool batteryPercent, int colour)
{
	SceInt ret = 0;
	
	char buf[64];
	int len = snprintf(buf, 64, menuConfig, batteryPercent, colour);
	
	if (R_FAILED(ret = writeFile("ux0:/data/vsh/config.cfg", buf, len)))
		return ret;
	
	return 0;
}

SceInt saveClockConfig(int cpuClock, int gpuClock)
{
	SceInt ret = 0;
	
	char game_config_path[35];
	snprintf(game_config_path, 35, "ux0:/data/vsh/titles/%s.cfg", titleID);
	
	char buf[64];
	int len = snprintf(buf, 64, clockConfig, cpuClock, gpuClock);
	
	if (R_FAILED(ret = writeFile(game_config_path, buf, len)))
		return ret;
	
	return 0;
}

SceInt loadConfig(SceVoid)
{
	SceInt ret = 0;
	
	char game_config_path[35];
	snprintf(game_config_path, 35, "ux0:/data/vsh/titles/%s.cfg", titleID);
	
	if (!(fileExists(game_config_path)))
	{
		c_clock = 2; // Default clock
		g_clock = 2; // Default clock
		return saveClockConfig(c_clock, g_clock);
	}
	
	if (!(fileExists("ux0:/data/vsh/config.cfg")))
	{
		// set these to the following by default:
		batteryPercent = 0;
		colour = 0;
		return saveMenuConfig(batteryPercent, colour);
	}
	
	char buf1[64], buf2[64];
	
	if (R_FAILED(ret = readFile(game_config_path, buf1, 64)))
		return ret;
	
	if (R_FAILED(ret = readFile("ux0:/data/vsh/config.cfg", buf2, 64)))
		return ret;
	
	sscanf(buf1, clockConfig, &c_clock, &g_clock);
	sscanf(buf2, menuConfig, &batteryPercent, &colour);
	
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