#include <libk/stdio.h>

#include <vitasdk.h>
#include <taihen.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "power.h"
#include "utils.h"

#define MAIN_MAX_ITEMS     9
#define BATTERY_MAX_ITEMS  3
#define VSH_MAIN_MENU      1
#define VSH_BATTERY_MENU   2
#define VSH_PROGRAM_MENU   3
#define HOOKS_NUM          9

static SceInt showVSH = 0;
static SceInt selection = 0;
static SceInt app_list = 0;

static SceUInt32 old_buttons, pressed_buttons;

static SceUID g_hooks[HOOKS_NUM];

static tai_hook_ref_t ref_hook[HOOKS_NUM];

static SceInt profile_max_battery[] = {111, 111, 111, 111};
static SceInt profile_default[] = {266, 166, 166, 111};
static SceInt profile_max_performance[] = {444, 222, 222, 166};
static SceInt profile_game[] = {444, 222, 222, 166};

static SceInt *profiles[4] = {profile_max_battery, profile_default, profile_game, profile_max_performance};

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

SceInt sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, SceInt sync) 
{
	drawSetFrameBuf(pParam);

	if (showVSH != 0)
	{
		if (batteryPercent)
			Power_DisplayBatteryPercentage();
		if (batteryLifeTime)
			Power_DisplayBatteryLifetime();
		if (batteryTemp)
			Power_DisplayBatteryTemp();
	}
	
	if (showVSH == VSH_MAIN_MENU)
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
	
	else if (showVSH == VSH_BATTERY_MENU)
	{		
		drawSetColour(WHITE, RGB_GREEN);
		drawStringCenter(100, "BATTERY OPTIONS");
		
		drawSetColour(WHITE, Config_GetVSHColour());
		
		drawStringCenter(130, "<- BACK");
		drawStringfCenter(162, "BATTERY PERCENT %s", batteryPercent? "enabled" : "disabled");
		drawStringfCenter(178, "BATTERY LIFETIME %s", batteryLifeTime? "enabled" : "disabled");
		drawStringfCenter(194, "BATTERY TEMP %s", batteryTemp? "enabled" : "disabled");
		
		switch(selection)
		{
			case 0:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(130, "<- BACK");
				break;
			case 1:
				drawSetColour(WHITE, SKYBLUE);
				drawStringfCenter(162, "BATTERY PERCENT %s", batteryPercent? "enabled" : "disabled");
				break;
			case 2:
				drawSetColour(WHITE, SKYBLUE);
				drawStringfCenter(178, "BATTERY LIFETIME %s", batteryLifeTime? "enabled" : "disabled");
				break;
			case 3:
				drawSetColour(WHITE, SKYBLUE);
				drawStringfCenter(194, "BATTERY TEMP %s", batteryTemp? "enabled" : "disabled");
				break;
		}
	}
	
	else if (showVSH == VSH_PROGRAM_MENU)
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
				drawStringCenter(162 + (16 *i), app_title[i]);
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
		}
	}
	
    return TAI_CONTINUE(SceInt, ref_hook[0], pParam, sync);
}

SceInt checkButtons(SceInt port, tai_hook_ref_t ref_hook, SceCtrlData *ctrl, SceInt count) 
{
	SceInt ret;

	if (ref_hook == 0)
		ret = 1;
	
	else
	{
		ret = TAI_CONTINUE(SceInt, ref_hook, port, ctrl, count);
		
		pressed_buttons = ctrl->buttons & ~old_buttons;

		if (showVSH == 1) // Main VSH Menu
		{
			if (pressed_buttons & SCE_CTRL_DOWN)
				selection += 1;
			else if (pressed_buttons & SCE_CTRL_UP)
				selection -= 1;
			
			if (selection == (MAIN_MAX_ITEMS + 1))
				selection = 0;
			if (selection == -1)
				selection = MAIN_MAX_ITEMS;
			
			if (selection == 0) 
			{
				if ((pressed_buttons & SCE_CTRL_LEFT) && (c_clock > 0))
				{
					c_clock--;
					scePowerSetArmClockFrequency(profiles[c_clock][0]);
					scePowerSetBusClockFrequency(profiles[c_clock][1]);
					Config_SaveClockConfig(c_clock, g_clock);
					Config_LoadConfig();
				}
				else if ((pressed_buttons & SCE_CTRL_RIGHT) && (c_clock < 3))
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
				if ((pressed_buttons & SCE_CTRL_LEFT) && (g_clock > 0))
				{
					g_clock--;
					scePowerSetGpuClockFrequency(profiles[g_clock][2]);
					scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
					Config_SaveClockConfig(c_clock, g_clock);
					Config_LoadConfig();
				}
				else if ((pressed_buttons & SCE_CTRL_RIGHT) && (g_clock < 3))
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
				if (pressed_buttons & SCE_CTRL_LEFT)
				{
					colour--;
					Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, colour);
					Config_LoadConfig();
				}
				else if (pressed_buttons & SCE_CTRL_RIGHT)
				{
					colour++;
					Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, colour);
					Config_LoadConfig();
				}
				
				if (colour > 8)
					colour = 0;
				else if (colour < 0)
					colour = 8;
			}
			else if ((selection == 3) && (pressed_buttons & SCE_CTRL_CROSS))
			{
				selection = 0;
				showVSH = VSH_BATTERY_MENU;
			}
			else if ((selection == 4) && (pressed_buttons & SCE_CTRL_CROSS))
			{
				selection = 0;
				showVSH = VSH_PROGRAM_MENU;
			}
			else if ((selection == 5) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestStandby();
			else if ((selection == 6) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestSuspend();
			else if ((selection == 7) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestColdReset();
			else if ((selection == 8) && (pressed_buttons & SCE_CTRL_CROSS))
				Utils_RestartVSH();
			else if (((selection == 9) && (pressed_buttons & SCE_CTRL_CROSS)) || (pressed_buttons & SCE_CTRL_CIRCLE))
			{
				selection = 0;
				showVSH = 0;
			}
			
			old_buttons = ctrl->buttons;
			ctrl->buttons = 0; // Disable controls
		}
		else if (showVSH == VSH_BATTERY_MENU)
		{
			if (pressed_buttons & SCE_CTRL_DOWN)
				selection += 1;
			else if (pressed_buttons & SCE_CTRL_UP)
				selection -= 1;
			
			if (selection == (BATTERY_MAX_ITEMS + 1))
				selection = 0;
			if (selection == -1)
				selection = BATTERY_MAX_ITEMS;
			
			if (((selection == 0) && (pressed_buttons & SCE_CTRL_CROSS)) || (pressed_buttons & SCE_CTRL_CIRCLE))
			{
				selection = 0;
				showVSH = VSH_MAIN_MENU;
			}
			else if ((selection == 1) && ((pressed_buttons & SCE_CTRL_LEFT) || (pressed_buttons & SCE_CTRL_RIGHT))) 
			{
				if (!batteryPercent)
					batteryPercent = SCE_TRUE;
				else 
					batteryPercent = SCE_FALSE;
				
				Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, colour);
				Config_LoadConfig();
			}
			else if ((selection == 2) && ((pressed_buttons & SCE_CTRL_LEFT) || (pressed_buttons & SCE_CTRL_RIGHT))) 
			{
				if (!batteryLifeTime)
					batteryLifeTime = SCE_TRUE;
				else 
					batteryLifeTime = SCE_FALSE;
					
				Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, colour);
				Config_LoadConfig();
			}
			else if ((selection == 3) && ((pressed_buttons & SCE_CTRL_LEFT) || (pressed_buttons & SCE_CTRL_RIGHT))) 
			{
				if (!batteryTemp)
					batteryTemp = SCE_TRUE;
				else 
					batteryTemp = SCE_FALSE;
					
				Config_SaveMenuConfig(batteryPercent, batteryLifeTime, batteryTemp, colour);
				Config_LoadConfig();
			}
			
			old_buttons = ctrl->buttons;
			ctrl->buttons = 0; // Disable controls
		}
		else if (showVSH == VSH_PROGRAM_MENU)
		{
			if (pressed_buttons & SCE_CTRL_DOWN)
				selection += 1;
			else if (pressed_buttons & SCE_CTRL_UP)
				selection -= 1;
			
			if (selection == (app_list + 1))
				selection = 0;
			if (selection == -1)
				selection = app_list;
			
			if (((selection == 0) && (pressed_buttons & SCE_CTRL_CROSS)) || (pressed_buttons & SCE_CTRL_CIRCLE))
			{
				selection = 0;
				showVSH = VSH_MAIN_MENU;
			}
			else if ((selection == 1) && (pressed_buttons & SCE_CTRL_CROSS)) 
			{
				if (strlen(app_title[0]) != 0)
					Utils_LaunchAppByUriExit(app_titleID[0]);
			}
			else if ((selection == 2) && (pressed_buttons & SCE_CTRL_CROSS)) 
			{
				if (strlen(app_title[1]) != 0)
					Utils_LaunchAppByUriExit(app_titleID[1]);
			}
			else if ((selection == 3) && (pressed_buttons & SCE_CTRL_CROSS)) 
			{
				if (strlen(app_title[2]) != 0)
					Utils_LaunchAppByUriExit(app_titleID[2]);
			}
			
			old_buttons = ctrl->buttons;
			ctrl->buttons = 0; // Disable controls
		}
		else
		{
			if ((ctrl->buttons & SCE_CTRL_LTRIGGER) && (ctrl->buttons & SCE_CTRL_RTRIGGER) && (ctrl->buttons & SCE_CTRL_START))
			{	
				sceAppMgrAppParamGetString(0, 12, titleID , 256); // Get titleID of current running application.
				FS_RecursiveMakeDir("ur0:/data/vsh/titles");
				Config_LoadConfig();

				if (c_clock == -1)
				{
					profile_game[0] = scePowerGetArmClockFrequency();
					profile_game[1] = scePowerGetBusClockFrequency();
					c_clock = 0;
				}
				
				if (g_clock == -1)
				{
					profile_game[2] = scePowerGetGpuClockFrequency();
					profile_game[3] = scePowerGetGpuXbarClockFrequency();
					g_clock = 0;
				}

				scePowerSetArmClockFrequency(profiles[c_clock][0]);
				scePowerSetBusClockFrequency(profiles[c_clock][1]);
				scePowerSetGpuClockFrequency(profiles[g_clock][2]);
				scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
				
				showVSH = VSH_MAIN_MENU;
			}
		}
	}
  
	return ret;
}

static SceInt scePowerSetClockFrequency_patched(tai_hook_ref_t ref_hook, SceInt port, SceInt freq)
{
	if (c_clock == -1)
		return TAI_CONTINUE(SceInt, ref_hook, freq);
	else
		return TAI_CONTINUE(SceInt, ref_hook, profiles[c_clock][port]);
	
	if (g_clock == -1)
		return TAI_CONTINUE(SceInt, ref_hook, freq);
	else
		return TAI_CONTINUE(SceInt, ref_hook, profiles[g_clock][port]);
}

static SceInt keys_patched1(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return checkButtons(port, ref_hook[1], ctrl, count);
}   

static SceInt keys_patched2(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return checkButtons(port, ref_hook[2], ctrl, count);
}   

static SceInt keys_patched3(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return checkButtons(port, ref_hook[3], ctrl, count);
}   

static SceInt keys_patched4(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return checkButtons(port, ref_hook[4], ctrl, count);
}   

static SceInt power_patched1(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(ref_hook[5], 0, freq);
}

static SceInt power_patched2(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(ref_hook[6], 1, freq);
}

static SceInt power_patched3(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(ref_hook[7], 2, freq);
}

static SceInt power_patched4(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(ref_hook[8], 3, freq);
}

SceVoid _start() __attribute__ ((weak, alias ("module_start")));
SceInt module_start(SceSize argc, const SceVoid *args) 
{	
	g_hooks[0] = Utils_TaiHookFunctionImport(&ref_hook[0], 0x7A410B64, sceDisplaySetFrameBuf_patched); // sceDisplaySetFrameBuf
	g_hooks[1] = Utils_TaiHookFunctionImport(&ref_hook[1], 0xA9C3CED6, keys_patched1);                 // sceCtrlPeekBufferPositive
	g_hooks[2] = Utils_TaiHookFunctionImport(&ref_hook[2], 0x15F81E8C, keys_patched2);                 // sceCtrlPeekBufferPositive2
	g_hooks[3] = Utils_TaiHookFunctionImport(&ref_hook[3], 0x67E7AB83, keys_patched3);                 // sceCtrlReadBufferPositive
	g_hooks[4] = Utils_TaiHookFunctionImport(&ref_hook[4], 0xC4226A3E, keys_patched4);                 // sceCtrlReadBufferPositive2
	g_hooks[5] = Utils_TaiHookFunctionImport(&ref_hook[5], 0x74DB5AE5, power_patched1);                // scePowerGetArmClockFrequency
	g_hooks[6] = Utils_TaiHookFunctionImport(&ref_hook[6], 0xB8D7B3FB, power_patched2);                // scePowerSetBusClockFrequency
	g_hooks[7] = Utils_TaiHookFunctionImport(&ref_hook[7], 0x717DB06C, power_patched3);                // scePowerSetGpuClockFrequency
	g_hooks[8] = Utils_TaiHookFunctionImport(&ref_hook[8], 0xA7739DBE, power_patched4);                // scePowerSetGpuXbarClockFrequency
	
	return SCE_KERNEL_START_SUCCESS;
}

SceInt module_stop(SceSize argc, const SceVoid *args) 
{
	// free hooks that didn't fail
	
	for (int i = (HOOKS_NUM - 1); i >= 0; i--)
	{
		if (R_SUCCEEDED(g_hooks[i])) 
			taiHookRelease(g_hooks[i], ref_hook[i]);
	}
	
	return SCE_KERNEL_STOP_SUCCESS;
}