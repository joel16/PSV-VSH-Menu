#include "config.h"
#include "draw.h"
#include "menus.h"
#include "power.h"
#include "utils.h"

#define MAIN_MAX_ITEMS     9
#define BATTERY_MAX_ITEMS  4
#define COLOUR_MAX_ITEMS   8

static SceInt app_list = 0;
SceInt selection = 0;

SceInt profile_max_battery[] = {111, 111, 111, 111};
SceInt profile_default[] = {266, 166, 166, 111};
SceInt profile_max_performance[] = {444, 222, 222, 166};
SceInt profile_game[] = {444, 222, 222, 166};

SceInt *profiles[4] = {profile_max_battery, profile_default, profile_game, profile_max_performance};

static char *colourStr[] = 
{
	"Red",
	"Green",
	"Blue",
	"Yellow",
	"Orange",
	"Purple",
	"Pink",
	"Black",
	"White"
};

static SceVoid Menu_DisplayMainMenu(SceVoid)
{
	drawSetColour(WHITE, RGB_GREEN);
	drawStringCenter(100, "PSV VSH MENU");
		
	drawSetColour(WHITE, Config_GetVSHColour());
		
	if (c_clock == 2)
		drawStringCenter(130, "CPU CLOCK default");
	else 
		drawStringfCenter(130, "CPU CLOCK %d/%d", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency());
	if (g_clock == 2)
		drawStringCenter(146, "GPU CLOCK default");
	else
		drawStringfCenter(146, "GPU CLOCK %d/%d", scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
		
	drawStringfCenter(162, "VSH MENU COLOUR %s", colourStr[colour]);
	drawStringCenter(178, "BATTERY OPTIONS ->");
	drawStringCenter(194, "LOAD PROGRAM ->");
	drawStringCenter(210, "SHUTDOWN DEVICE");
	drawStringCenter(226, "SUSPEND DEVICE");
	drawStringCenter(242, "REBOOT DEVICE");
	drawStringCenter(258, "RESTART VSH");
	drawStringCenter(274, "EXIT");
		
	switch(selection)
	{
		case 0:
			drawSetColour(WHITE, SKYBLUE);
			if (c_clock == 2)
				drawStringCenter(130, "CPU CLOCK default");
			else
				drawStringfCenter(130, "CPU CLOCK %d/%d", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency());
			break;
		case 1:
			drawSetColour(WHITE, SKYBLUE);
			if (g_clock == 2)
				drawStringCenter(146, "GPU CLOCK default");
			else
				drawStringfCenter(146, "GPU CLOCK %d/%d", scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
			break;
		case 2:
			drawSetColour(WHITE, SKYBLUE);
			drawStringfCenter(162, "VSH MENU COLOUR %s", colourStr[colour]);
			break;
		case 3:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(178, "BATTERY OPTIONS ->");
			break;
		case 4:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(194, "LOAD PROGRAM ->");
			break;
		case 5:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(210, "SHUTDOWN DEVICE");
			break;
		case 6:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(226, "SUSPEND DEVICE");
			break;
		case 7:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(242, "REBOOT DEVICE");
			break;
		case 8:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(258, "RESTART VSH");
			break;
		case 9:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(274, "EXIT");
			break;
	}
}

static SceVoid Menu_DisplayBatteryMenu(SceVoid)
{
	drawSetColour(WHITE, RGB_GREEN);
	drawStringCenter(100, "BATTERY OPTIONS");
		
	drawSetColour(WHITE, Config_GetVSHColour());
		
	drawStringCenter(130, "<- BACK");
	drawStringfCenter(162, "KEEP BATTERY DISPLAY %s", batteryDisplay? "enabled" : "disabled");
	drawStringfCenter(178, "BATTERY PERCENT %s", batteryPercent? "enabled" : "disabled");
	drawStringfCenter(194, "BATTERY LIFETIME %s", batteryLifeTime? "enabled" : "disabled");
	drawStringfCenter(210, "BATTERY TEMP %s", batteryTemp? "enabled" : "disabled");
		
	switch(selection)
	{
		case 0:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(130, "<- BACK");
			break;
		case 1:
			drawSetColour(WHITE, SKYBLUE);
			drawStringfCenter(162, "KEEP BATTERY DISPLAY %s", batteryDisplay? "enabled" : "disabled");
			break;
		case 2:
			drawSetColour(WHITE, SKYBLUE);
			drawStringfCenter(178, "BATTERY PERCENT %s", batteryPercent? "enabled" : "disabled");
			break;
		case 3:
			drawSetColour(WHITE, SKYBLUE);
			drawStringfCenter(194, "BATTERY LIFETIME %s", batteryLifeTime? "enabled" : "disabled");
			break;
		case 4:
			drawSetColour(WHITE, SKYBLUE);
			drawStringfCenter(210, "BATTERY TEMP %s", batteryTemp? "enabled" : "disabled");
			break;
	}
}

static SceVoid Menu_DisplayProgramMenu(SceVoid)
{
	drawSetColour(WHITE, RGB_GREEN);
	drawStringCenter(100, "LOAD PROGRAM");
		
	drawSetColour(WHITE, Config_GetVSHColour());
	
	drawStringCenter(130, "<- BACK");
	
	for (SceInt i = 0; i < 10; i++)
	{
		if (strlen(app_title[i]) != 0)
		{
			app_list = (i + 1); // increment app list
			drawStringCenter(162 + (16 * i), app_title[i]);
		}
	}
		
	switch(selection)
	{
		case 0:
			drawSetColour(WHITE, SKYBLUE);
			drawStringCenter(130, "<- BACK");
			break;
		case 1:
			if (strlen(app_title[0]) != 0)
			{
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(162, app_title[0]);
			}
			break;
		case 2:
			if (strlen(app_title[1]) != 0)
			{
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(178, app_title[1]);
			}
			break;
		case 3:
			if (strlen(app_title[2]) != 0)
			{
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(194, app_title[2]);
			}
			break;
		case 4:
			if (strlen(app_title[3]) != 0)
			{
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(210, app_title[3]);
			}
			break;
		case 5:
			if (strlen(app_title[4]) != 0)
			{
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(226, app_title[4]);
			}
			break;
	}
}

SceVoid Menu_Display(SceBool vertical_blank)
{
	if (vertical_blank)
		drawInit();
	
	if (showVSH == VSH_MAIN_MENU)
		Menu_DisplayMainMenu();
	else if (showVSH == VSH_BATTERY_MENU)
		Menu_DisplayBatteryMenu();
	else if (showVSH == VSH_PROGRAM_MENU)
		Menu_DisplayProgramMenu();

	if (vertical_blank)
		sceDisplayWaitVblankStart();
}

SceInt Menu_HandleControls(SceUInt32 pad)
{
	if (showVSH == 1) // Main VSH Menu
	{
		if (pad & SCE_CTRL_DOWN)
			selection += 1;
		else if (pad & SCE_CTRL_UP)
			selection -= 1;
			
		if (selection == (MAIN_MAX_ITEMS + 1))
			selection = 0;
		if (selection == -1)
			selection = MAIN_MAX_ITEMS;
			
		if (selection == 0) 
		{
			if ((pad & SCE_CTRL_LEFT) && (c_clock > 0))
			{
				c_clock--;
				scePowerSetArmClockFrequency(profiles[c_clock][0]);
				scePowerSetBusClockFrequency(profiles[c_clock][1]);
				Config_SaveClockConfig(c_clock, g_clock);
				Config_LoadConfig();
			}
			else if ((pad & SCE_CTRL_RIGHT) && (c_clock < 3))
			{
				c_clock++;
				scePowerSetArmClockFrequency(profiles[c_clock][0]);
				scePowerSetBusClockFrequency(profiles[c_clock][1]);
				Config_SaveClockConfig(c_clock, g_clock);
				Config_LoadConfig();
			}
		}
		else if (selection == 1)
		{
			if ((pad & SCE_CTRL_LEFT) && (g_clock > 0))
			{
				g_clock--;
				scePowerSetGpuClockFrequency(profiles[g_clock][2]);
				scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
				Config_SaveClockConfig(c_clock, g_clock);
				Config_LoadConfig();
			}
			else if ((pad & SCE_CTRL_RIGHT) && (g_clock < 3))
			{
				g_clock++;
				scePowerSetGpuClockFrequency(profiles[g_clock][2]);
				scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
				Config_SaveClockConfig(c_clock, g_clock);
				Config_LoadConfig();
			}
		}
		else if (selection == 2)
		{
			if (pad & SCE_CTRL_LEFT)
			{
				if (colour > 0)
					colour--;
				else 
					colour = COLOUR_MAX_ITEMS;
				
				Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, batteryDisplay, colour);
				Config_LoadConfig();
			}
			else if (pad & SCE_CTRL_RIGHT)
			{
				if (colour < COLOUR_MAX_ITEMS)
					colour++;
				else 
					colour = 0;

				Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, batteryDisplay, colour);
				Config_LoadConfig();
			}
		}
		else if ((selection == 3) && (pad & SCE_CTRL_CROSS))
		{
			selection = 0;
			showVSH = VSH_BATTERY_MENU;
		}
		else if ((selection == 4) && (pad & SCE_CTRL_CROSS))
		{
			selection = 0;
			showVSH = VSH_PROGRAM_MENU;
		}
		else if ((selection == 5) && (pad & SCE_CTRL_CROSS)) 
			scePowerRequestStandby();
		else if ((selection == 6) && (pad & SCE_CTRL_CROSS)) 
			scePowerRequestSuspend();
		else if ((selection == 7) && (pad & SCE_CTRL_CROSS)) 
			scePowerRequestColdReset();
		else if ((selection == 8) && (pad & SCE_CTRL_CROSS))
			Utils_RestartVSH();
		else if (((selection == 9) && (pad & SCE_CTRL_CROSS)) || (pad & SCE_CTRL_CIRCLE))
		{
			selection = 0;
			showVSH = 0;
		}
	}
	else if (showVSH == VSH_BATTERY_MENU)
	{
		if (pad & SCE_CTRL_DOWN)
			selection += 1;
		else if (pad & SCE_CTRL_UP)
			selection -= 1;
			
		if (selection == (BATTERY_MAX_ITEMS + 1))
			selection = 0;
		if (selection == -1)
			selection = BATTERY_MAX_ITEMS;
		
		if (((selection == 0) && (pad & SCE_CTRL_CROSS)) || (pad & SCE_CTRL_CIRCLE))
		{
			selection = 0;
			showVSH = VSH_MAIN_MENU;
		}
		else
		{
			if ((pad & SCE_CTRL_LEFT) || (pad & SCE_CTRL_RIGHT))
			{
				switch(selection)
				{
					case 1:
						batteryDisplay = !batteryDisplay;
						break;
					case 2:
						batteryPercent = !batteryPercent;
						break;
					case 3:
						batteryLifeTime = !batteryLifeTime;
						break;
					case 4:
						batteryTemp = !batteryTemp;
						break;
				}

				Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, batteryDisplay, colour);
				Config_LoadConfig();
			}
		}
	}
	else if (showVSH == VSH_PROGRAM_MENU)
	{
		if (pad & SCE_CTRL_DOWN)
			selection += 1;
		else if (pad & SCE_CTRL_UP)
			selection -= 1;
			
		if (selection == (app_list + 1))
			selection = 0;
		if (selection == -1)
			selection = app_list;
			
		if (selection == 0)
		{
			if ((pad & SCE_CTRL_CROSS) || (pad & SCE_CTRL_CIRCLE))
			{
				selection = 0;
				showVSH = VSH_MAIN_MENU;
			}
		}
		else
		{
			if (pad & SCE_CTRL_CROSS)
			{
				if (strlen(app_title[selection - 1]) != 0)
					Utils_LaunchAppByUriExit(app_titleID[selection - 1]);
			}		
		}
	}

	return 0;
}