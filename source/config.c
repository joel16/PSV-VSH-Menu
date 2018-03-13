#include <libk/stdio.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "utils.h"

const char *menuConfig =
	"display_battery = %d\n"
	"display_battery_lifetime = %d\n"
	"display_battery_temp = %d\n"
	"bar_colour = %d\n";

const char *clockConfig =
	"CPU_clock = %d\n"
	"GPU_clock = %d\n";
	
const char *launcherConfig =
	"[0] title: %s titleID: %s\n"
	"[1] title: %s titleID: %s\n"
	"[2] title: %s titleID: %s\n"
	"[3] title: %s titleID: %s\n"
	"[4] title: %s titleID: %s\n";

SceInt Config_SaveMenuConfig(SceBool batteryPercent, SceBool batteryLifeTime, SceBool batteryTemp, int colour)
{
	SceInt ret = 0;
	
	char *menu_config_path = (char *)Utils_SceMalloc(25);
	sceClibSnprintf(menu_config_path, 25, "ur0:/data/vsh/config.cfg");
	
	char *buf = (char *)Utils_SceMalloc(128);
	SceInt len = sceClibSnprintf(buf, 128, menuConfig, batteryPercent, batteryLifeTime, batteryTemp, colour);
	
	if (R_FAILED(ret = FS_WriteFile(menu_config_path, buf, len)))
	{
		Utils_SceFree(buf);
		Utils_SceFree(menu_config_path);
		return ret;
	}
	
	Utils_SceFree(buf);
	Utils_SceFree(menu_config_path);
	return 0;
}

SceInt Config_SaveClockConfig(int cpuClock, int gpuClock)
{
	SceInt ret = 0;
	
	char *game_config_path = (char *)Utils_SceMalloc(35);
	sceClibSnprintf(game_config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);
	
	char *buf = (char *)Utils_SceMalloc(64);
	SceInt len = sceClibSnprintf(buf, 64, clockConfig, cpuClock, gpuClock);
	
	if (R_FAILED(ret = FS_WriteFile(game_config_path, buf, len)))
	{
		Utils_SceFree(buf);
		Utils_SceFree(game_config_path);
		return ret;
	}
	
	Utils_SceFree(buf);
	Utils_SceFree(game_config_path);
	return 0;
}

static SceInt Config_SaveLauncherConfig()
{
	SceInt ret = 0;
	
	char *launcher_config_path = (char *)Utils_SceMalloc(27);
	sceClibSnprintf(launcher_config_path, 27, "ur0:/data/vsh/launcher.cfg");
	
	char *buf = (char *)Utils_SceMalloc(256);
	
	// set these to the following by default:
	sceClibSnprintf(app_title[0], 9, "Settings");
	sceClibSnprintf(app_titleID[0], 14, "settings_dlg:");
	sceClibSnprintf(app_title[1], 10, "VitaShell");
	sceClibSnprintf(app_titleID[1], 10, "VITASHELL");
	sceClibSnprintf(app_title[2], 11, "Adrenaline");
	sceClibSnprintf(app_titleID[2], 10, "PSPEMUCFW");
	sceClibSnprintf(app_title[3], 5, "VHBB");
	sceClibSnprintf(app_titleID[3], 10, "VHBB00001");
	sceClibSnprintf(app_title[4], 10, "VITAident");
	sceClibSnprintf(app_titleID[4], 10, "VID000162");
	
	SceInt len = sceClibSnprintf(buf, 256, launcherConfig, app_title[0], app_titleID[0], app_title[1], app_titleID[1], 
		app_title[2], app_titleID[2], app_title[3], app_titleID[3], app_title[4], app_titleID[4]);
	
	if (R_FAILED(ret = FS_WriteFile(launcher_config_path, buf, len)))
	{
		Utils_SceFree(buf);
		Utils_SceFree(launcher_config_path);
		return ret;
	}
	
	Utils_SceFree(buf);
	Utils_SceFree(launcher_config_path);
	return 0;
}

SceInt Config_LoadConfig(SceVoid)
{
	SceInt ret = 0;
	
	char *game_config_path = (char *)Utils_SceMalloc(35);
	char *menu_config_path = (char *)Utils_SceMalloc(25);
	char *launcher_config_path = (char *)Utils_SceMalloc(27);
	
	sceClibSnprintf(game_config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);
	sceClibSnprintf(menu_config_path, 25, "ur0:/data/vsh/config.cfg");
	sceClibSnprintf(launcher_config_path, 27, "ur0:/data/vsh/launcher.cfg");
	
	if (!(FS_FileExists(game_config_path)))
	{
		c_clock = 2; // Default clock
		g_clock = 2; // Default clock
		Config_SaveClockConfig(c_clock, g_clock);
	}
	
	if (!(FS_FileExists(menu_config_path)))
	{
		// set these to the following by default:
		batteryPercent = 0;
		batteryLifeTime = 0;
		batteryTemp = 0;
		colour = 0;
		Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, colour);
	}
	
	if (!(FS_FileExists(launcher_config_path)))	
		Config_SaveLauncherConfig();
	
	char *buf1 = (char *)Utils_SceMalloc(64);
	char *buf2 = (char *)Utils_SceMalloc(128);
	char *buf3 = (char *)Utils_SceMalloc(256);
	
	if (R_FAILED(ret = FS_ReadFile(game_config_path, buf1, 64)))
	{
		Utils_SceFree(buf3);
		Utils_SceFree(buf2);
		Utils_SceFree(buf1);
		Utils_SceFree(menu_config_path);
		Utils_SceFree(game_config_path);
		Utils_SceFree(launcher_config_path);
		return ret;
	}
	
	if (R_FAILED(ret = FS_ReadFile(menu_config_path, buf2, 128)))
	{
		Utils_SceFree(buf3);
		Utils_SceFree(buf2);
		Utils_SceFree(buf1);
		Utils_SceFree(menu_config_path);
		Utils_SceFree(game_config_path);
		Utils_SceFree(launcher_config_path);
		return ret;
	}
	
	if (R_FAILED(ret = FS_ReadFile(launcher_config_path, buf3, 256)))
	{
		Utils_SceFree(buf3);
		Utils_SceFree(buf2);
		Utils_SceFree(buf1);
		Utils_SceFree(menu_config_path);
		Utils_SceFree(game_config_path);
		Utils_SceFree(launcher_config_path);
		return ret;
	}
	
	sscanf(buf1, clockConfig, &c_clock, &g_clock);
	sscanf(buf2, menuConfig, &batteryPercent, &batteryLifeTime, &batteryTemp, &colour);
	sscanf(buf3, launcherConfig, app_title[0], app_titleID[0], app_title[1], app_titleID[1], 
		app_title[2], app_titleID[2], app_title[3], app_titleID[3], app_title[4], app_titleID[4]);
	
	Utils_SceFree(buf3);
	Utils_SceFree(buf2);
	Utils_SceFree(buf1);
	Utils_SceFree(menu_config_path);
	Utils_SceFree(game_config_path);
	Utils_SceFree(launcher_config_path);
	return 0;
}

int Config_GetVSHColour()
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