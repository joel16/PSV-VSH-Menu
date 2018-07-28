#include <libk/stdio.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "utils.h"

static const char *menuConfig =
	"display_battery_percent = %d\n"
	"display_battery_lifetime = %d\n"
	"display_battery_temp = %d\n"
	"display_battery_data = %d\n"
	"bar_colour = %d\n"
	"display_fps = %d\n"
	"display_fps_data = %d\n"
	"display_clock = %d\n"
	"display_clock_data = %d\n";

static const char *customColourConfig =
	"TITLE_BG_COL = %x\n"
	"BG_COL = %x\n"
	"CURSOR_COL = %x\n"
	"TITLE_TEXT_COL = %x\n"
	"TEXT_COL = %x\n";

static const char *clockConfig =
	"CLOCK_SET = %d\n"
	"CPU_clock = %d\n"
	"GPU_clock = %d\n"
	"Refresh_interval = %d\n";
	
static const char *launcherConfig =
	"[0] title: %s titleID: %s\n"
	"[1] title: %s titleID: %s\n"
	"[2] title: %s titleID: %s\n"
	"[3] title: %s titleID: %s\n"
	"[4] title: %s titleID: %s\n";

SceInt Config_SaveMenuConfig(Menu_Config_t Menu_Config)
{
	SceInt ret = 0;
	
	char *menu_config_path = (char *)Utils_SceMalloc(25);
	snprintf(menu_config_path, 25, "ur0:/data/vsh/config.cfg");
	
	char *buf = (char *)Utils_SceMalloc(256);
	SceInt len = snprintf(buf, 256, menuConfig, Menu_Config.battery_percent, Menu_Config.battery_lifetime, Menu_Config.battery_temp, 
		Menu_Config.battery_keep_display, Menu_Config.colour, Menu_Config.fps_display, Menu_Config.fps_keep_display, Menu_Config.clock_display, Menu_Config.clock_keep_display);
	
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

static SceInt Config_SaveColourConfig(Custom_Colour_t Custom_Colour)
{
	SceInt ret = 0;
	
	char *colour_config_path = (char *)Utils_SceMalloc(39);
	snprintf(colour_config_path, 39, "ur0:/data/vsh/custom_colour_config.cfg");
	
	char *buf = (char *)Utils_SceMalloc(256);
	SceInt len = snprintf(buf, 256, customColourConfig, Custom_Colour.title_bg_col, Custom_Colour.bg_col, Custom_Colour.cursor_col, 
		Custom_Colour.title_text_col, Custom_Colour.text_col);
	
	if (R_FAILED(ret = FS_WriteFile(colour_config_path, buf, len)))
	{
		Utils_SceFree(buf);
		Utils_SceFree(colour_config_path);
		return ret;
	}
	
	Utils_SceFree(buf);
	Utils_SceFree(colour_config_path);
	return 0;
}

SceInt Config_SaveClockConfig(Clock_Config_t Clock_Config)
{
	SceInt ret = 0;
	
	char *game_config_path = (char *)Utils_SceMalloc(35);
	snprintf(game_config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);
	
	char *buf = (char *)Utils_SceMalloc(128);
	SceInt len = snprintf(buf, 128, clockConfig, Clock_Config.clock_set, Clock_Config.c_clock, Clock_Config.g_clock, Clock_Config.refresh_interval);
	
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
	snprintf(launcher_config_path, 27, "ur0:/data/vsh/launcher.cfg");
	
	char *buf = (char *)Utils_SceMalloc(256);
	
	// set these to the following by default:
	snprintf(app_title[0], 9, "Settings");
	snprintf(app_titleID[0], 14, "settings_dlg:");
	snprintf(app_title[1], 10, "VitaShell");
	snprintf(app_titleID[1], 10, "VITASHELL");
	snprintf(app_title[2], 11, "Adrenaline");
	snprintf(app_titleID[2], 10, "PSPEMUCFW");
	snprintf(app_title[3], 5, "VHBB");
	snprintf(app_titleID[3], 10, "VHBB00001");
	snprintf(app_title[4], 10, "VITAident");
	snprintf(app_titleID[4], 10, "VID000162");
	
	SceInt len = snprintf(buf, 256, launcherConfig, app_title[0], app_titleID[0], app_title[1], app_titleID[1], 
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

	char *config_path = (char *)Utils_SceMalloc(39);
	char *buf = (char *)Utils_SceMalloc(256);
	
	// Application config
	snprintf(config_path, 35, "ur0:/data/vsh/titles/%s.cfg", titleID);

	if (!(FS_FileExists(config_path)))
	{
		Clock_Config.clock_set = SCE_FALSE;
		Clock_Config.c_clock = 2; // Default clock
		Clock_Config.g_clock = 2; // Default clock
		Clock_Config.refresh_interval = 4; // Default refresh rate = 30 seconds
		Config_SaveClockConfig(Clock_Config);
	}

	if (R_FAILED(ret = FS_ReadFile(config_path, buf, 128)))
	{
		Utils_SceFree(config_path);
		Utils_SceFree(buf);
		return ret;
	}

	sscanf(buf, clockConfig, &Clock_Config.clock_set, &Clock_Config.c_clock, &Clock_Config.g_clock, &Clock_Config.refresh_interval);
	memset(config_path, 0, 39);
	memset(buf, 0, 256);

	// Menu config
	snprintf(config_path, 25, "ur0:/data/vsh/config.cfg");

	if (!(FS_FileExists(config_path)))
	{
		// set these to the following by default:
		Menu_Config.colour = 0;
		Menu_Config.battery_percent = SCE_FALSE;
		Menu_Config.battery_lifetime = SCE_FALSE;
		Menu_Config.battery_temp = SCE_FALSE;
		Menu_Config.battery_keep_display = SCE_FALSE;
		Menu_Config.fps_display = SCE_FALSE;
		Menu_Config.fps_keep_display = SCE_FALSE;
		Menu_Config.clock_display = SCE_FALSE;
		Menu_Config.clock_keep_display = SCE_FALSE;
		Config_SaveMenuConfig(Menu_Config);
	}

	if (R_FAILED(ret = FS_ReadFile(config_path, buf, 256)))
	{
		Utils_SceFree(config_path);
		Utils_SceFree(buf);
		return ret;
	}

	sscanf(buf, menuConfig, &Menu_Config.battery_percent, &Menu_Config.battery_lifetime, &Menu_Config.battery_temp, &Menu_Config.battery_keep_display, 
		&Menu_Config.colour, &Menu_Config.fps_display, &Menu_Config.fps_keep_display, &Menu_Config.clock_display, &Menu_Config.clock_keep_display);
	memset(config_path, 0, 39);
	memset(buf, 0, 256);

	// Custom colour config
	snprintf(config_path, 39, "ur0:/data/vsh/custom_colour_config.cfg");

	if (!(FS_FileExists(config_path)))
	{
		Custom_Colour.title_bg_col = 0x00333333;
		Custom_Colour.bg_col = 0x00333333;
		Custom_Colour.cursor_col = 0x80FFFFFF;
		Custom_Colour.title_text_col = 0x0000FF00;
		Custom_Colour.text_col = 0x0000FF00;
		Config_SaveColourConfig(Custom_Colour);
	}

	if (R_FAILED(ret = FS_ReadFile(config_path, buf, 256)))
	{
		Utils_SceFree(config_path);
		Utils_SceFree(buf);
		return ret;
	}

	sscanf(buf, customColourConfig, &Custom_Colour.title_bg_col, &Custom_Colour.bg_col, &Custom_Colour.cursor_col, &Custom_Colour.title_text_col,
		&Custom_Colour.text_col);
	memset(config_path, 0, 39);
	memset(buf, 0, 256);

	// Launcher config
	snprintf(config_path, 27, "ur0:/data/vsh/launcher.cfg");
	
	if (!(FS_FileExists(config_path)))	
		Config_SaveLauncherConfig();
	
	if (R_FAILED(ret = FS_ReadFile(config_path, buf, 256)))
	{
		Utils_SceFree(config_path);
		Utils_SceFree(buf);
		return ret;
	}

	sscanf(buf, launcherConfig, app_title[0], app_titleID[0], app_title[1], app_titleID[1], 
		app_title[2], app_titleID[2], app_title[3], app_titleID[3], app_title[4], app_titleID[4]);

	Utils_SceFree(config_path);
	Utils_SceFree(buf);
	return 0;
}

SceInt Config_GetVSHColour(SceVoid)
{
	SceInt col = 0;
	
	switch (Menu_Config.colour)
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

SceUInt64 Config_GetInterval(SceVoid)
{
	SceUInt64 interval = 0;

	switch(Clock_Config.refresh_interval)
	{
		case 0:
			interval = 5000000;
			break;
		case 1:
			interval = 10000000;
			break;
		case 2:
			interval = 20000000;
			break;
		case 3:
			interval = 30000000;
			break;
		case 4:
			interval = 45000000;
			break;
		case 5:
			interval = 60000000;
			break;
	}

	return interval;
}
