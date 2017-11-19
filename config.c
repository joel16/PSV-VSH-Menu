#include <libk/stdio.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "utils.h"

const char * menuConfig =
	"display_battery = %d\n"
	"display_battery_lifetime = %d\n"
	"display_battery_temp = %d\n"
	"bar_colour = %d\n";

const char * clockConfig =
	"CPU_clock = %d\n"
	"GPU_clock = %d\n";

SceInt saveMenuConfig(SceBool batteryPercent, SceBool batteryLifeTime, SceBool batteryTemp, int colour)
{
	SceInt ret = 0;
	
	char menu_config_path[25];
	isUx0? snprintf(menu_config_path, 25, "ux0:/data/vsh/config.cfg") : snprintf(menu_config_path, 25, "ur0:/data/vsh/config.cfg");
	
	char buf[128];
	int len = snprintf(buf, 128, menuConfig, batteryPercent, batteryLifeTime, batteryTemp, colour);
	
	if (R_FAILED(ret = writeFile(menu_config_path, buf, len)))
		return ret;
	
	return 0;
}

SceInt saveClockConfig(int cpuClock, int gpuClock)
{
	SceInt ret = 0;
	
	char game_config_path[35];
	isUx0? snprintf(game_config_path, 35, "ux0:/data/vsh/titles/%s.cfg", titleID) : snprintf(game_config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);
	
	char buf[64];
	int len = snprintf(buf, 64, clockConfig, cpuClock, gpuClock);
	
	if (R_FAILED(ret = writeFile(game_config_path, buf, len)))
		return ret;
	
	return 0;
}

SceInt loadConfig(SceVoid)
{
	SceInt ret = 0;
	
	char game_config_path[35], menu_config_path[25];
	
	isUx0? snprintf(game_config_path, 35, "ux0:/data/vsh/titles/%s.cfg", titleID) : snprintf(game_config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);
	isUx0? snprintf(menu_config_path, 25, "ux0:/data/vsh/config.cfg") : snprintf(menu_config_path, 25, "ur0:/data/vsh/config.cfg");
	
	if (!(fileExists(game_config_path)))
	{
		c_clock = 2; // Default clock
		g_clock = 2; // Default clock
		saveClockConfig(c_clock, g_clock);
	}
	
	if (!(fileExists(menu_config_path)))
	{
		// set these to the following by default:
		batteryPercent = 0;
		batteryLifeTime = 0;
		batteryTemp = 0;
		colour = 0;
		saveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, colour);
	}
	
	char buf1[64], buf2[128];
	
	if (R_FAILED(ret = readFile(game_config_path, buf1, 64)))
		return ret;
	
	if (R_FAILED(ret = readFile(menu_config_path, buf2, 128)))
		return ret;
	
	sscanf(buf1, clockConfig, &c_clock, &g_clock);
	sscanf(buf2, menuConfig, &batteryPercent, &batteryLifeTime, &batteryTemp, &colour);
	
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