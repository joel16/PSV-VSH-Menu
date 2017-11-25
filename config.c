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
	
const char * launcherConfig =
	"[0] title: %s titleID: %s\n"
	"[1] title: %s titleID: %s\n";

SceInt saveMenuConfig(SceBool batteryPercent, SceBool batteryLifeTime, SceBool batteryTemp, int colour)
{
	SceInt ret = 0;
	
	char * menu_config_path = (char *)_malloc(25);
	isUx0? snprintf(menu_config_path, 25, "ux0:/data/vsh/config.cfg") : snprintf(menu_config_path, 25, "ur0:/data/vsh/config.cfg");
	
	char * buf = (char *)_malloc(128);
	SceInt len = snprintf(buf, 128, menuConfig, batteryPercent, batteryLifeTime, batteryTemp, colour);
	
	if (R_FAILED(ret = writeFile(menu_config_path, buf, len)))
	{
		_free(buf);
		_free(menu_config_path);
		return ret;
	}
	
	_free(buf);
	_free(menu_config_path);
	return 0;
}

SceInt saveClockConfig(int cpuClock, int gpuClock)
{
	SceInt ret = 0;
	
	char * game_config_path = (char *)_malloc(35);
	isUx0? snprintf(game_config_path, 35, "ux0:/data/vsh/titles/%s.cfg", titleID) : snprintf(game_config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);
	
	char * buf = (char *)_malloc(64);
	SceInt len = snprintf(buf, 64, clockConfig, cpuClock, gpuClock);
	
	if (R_FAILED(ret = writeFile(game_config_path, buf, len)))
	{
		_free(buf);
		_free(game_config_path);
		return ret;
	}
	
	_free(buf);
	_free(game_config_path);
	return 0;
}

SceInt saveLauncherConfig()
{
	SceInt ret = 0;
	
	char * launcher_config_path = (char *)_malloc(27);
	isUx0? snprintf(launcher_config_path, 27, "ux0:/data/vsh/launcher.cfg") : snprintf(launcher_config_path, 27, "ur0:/data/vsh/launcher.cfg");
	
	char * buf = (char *)_malloc(128);
	
	// set these to the following by default:
	snprintf(app_title[0], 9, "Settings");
	snprintf(app_titleID[0], 30, "settings_dlg:");
	snprintf(app_title[1], 10, "VitaShell");
	snprintf(app_titleID[1], 10, "VITASHELL");
	
	SceInt len = snprintf(buf, 128, launcherConfig, app_title[0], app_titleID[0], app_title[1], app_titleID[1]);
	
	if (R_FAILED(ret = writeFile(launcher_config_path, buf, len)))
	{
		_free(buf);
		_free(launcher_config_path);
		return ret;
	}
	
	_free(buf);
	_free(launcher_config_path);
	return 0;
}

SceInt loadConfig(SceVoid)
{
	SceInt ret = 0;
	
	char * game_config_path = (char *)_malloc(35);
	char * menu_config_path = (char *)_malloc(25);
	char * launcher_config_path = (char *)_malloc(27);
	
	isUx0? snprintf(game_config_path, 35, "ux0:/data/vsh/titles/%s.cfg", titleID) : snprintf(game_config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);
	isUx0? snprintf(menu_config_path, 25, "ux0:/data/vsh/config.cfg") : snprintf(menu_config_path, 25, "ur0:/data/vsh/config.cfg");
	isUx0? snprintf(launcher_config_path, 27, "ux0:/data/vsh/launcher.cfg") : snprintf(launcher_config_path, 27, "ur0:/data/vsh/launcher.cfg");
	
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
	
	if (!(fileExists(launcher_config_path)))	
		saveLauncherConfig();
	
	char * buf1 = (char *)_malloc(64);
	char * buf2 = (char *)_malloc(128);
	char * buf3 = (char *)_malloc(256);
	
	if (R_FAILED(ret = readFile(game_config_path, buf1, 64)))
	{
		_free(buf3);
		_free(buf2);
		_free(buf1);
		_free(menu_config_path);
		_free(game_config_path);
		_free(launcher_config_path);
		return ret;
	}
	
	if (R_FAILED(ret = readFile(menu_config_path, buf2, 128)))
	{
		_free(buf3);
		_free(buf2);
		_free(buf1);
		_free(menu_config_path);
		_free(game_config_path);
		_free(launcher_config_path);
		return ret;
	}
	
	if (R_FAILED(ret = readFile(launcher_config_path, buf3, 256)))
	{
		_free(buf3);
		_free(buf2);
		_free(buf1);
		_free(menu_config_path);
		_free(game_config_path);
		_free(launcher_config_path);
		return ret;
	}
	
	sscanf(buf1, clockConfig, &c_clock, &g_clock);
	sscanf(buf2, menuConfig, &batteryPercent, &batteryLifeTime, &batteryTemp, &colour);
	sscanf(buf3, launcherConfig, app_title[0], app_titleID[0], app_title[1], app_titleID[1]);
	
	_free(buf3);
	_free(buf2);
	_free(buf1);
	_free(menu_config_path);
	_free(game_config_path);
	_free(launcher_config_path);
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