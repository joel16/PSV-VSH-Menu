#include <libk/stdio.h>

#include <vitasdk.h>
#include <taihen.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "utils.h"

#define MAIN_MAX_ITEMS 10
#define APP_MAX_ITEMS 2

static SceInt showVSH = 0;
static SceInt selection = 0;

static SceUInt32 old_buttons, pressed_buttons;

static SceUID g_hooks[11];

static tai_hook_ref_t ref_hook0, ref_hook1, ref_hook2, ref_hook3, ref_hook4;
static tai_hook_ref_t power_hook1, power_hook2, power_hook3, power_hook4;

static SceInt profile_max_battery[] = {111, 111, 111, 111};
static SceInt profile_default[] = {266, 166, 166, 111};
static SceInt profile_max_performance[] = {444, 222, 222, 166};
static SceInt profile_game[] = {444, 222, 222, 166};

static SceInt * profiles[4] = {profile_max_battery, profile_default, profile_game, profile_max_performance};

static char * colourStr[] = 
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

SceInt sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf * pParam, SceInt sync) 
{
	drawSetFrameBuf(pParam);
	
	if (showVSH == 1)
	{	
		if (batteryPercent)
		{
			int percent = scePowerGetBatteryLifePercent();
			drawSetColour(WHITE, getColour());
		
			if (percent == 100)
				drawStringf(880, 0, "%d %%", percent);
			else
				drawStringf(896, 0, "%d %%", percent);
		}
		
		drawSetColour(WHITE, RGB_GREEN);
		drawStringCenter(100, "PSV VSH MENU");
		
		drawSetColour(WHITE, getColour());
		
		if (c_clock == 2)
			drawStringfCenter(130, "CPU CLOCK default");
		else 
			drawStringfCenter(130, "CPU CLOCK %d/%d", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency());
		if (g_clock == 2)
			drawStringfCenter(146, "GPU CLOCK default");
		else
			drawStringfCenter(146, "GPU CLOCK %d/%d", scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
		
		drawStringfCenter(162, "BATTERY STATUS %s", batteryPercent? "enabled" : "disabled");
		drawStringfCenter(178, "VSH MENU COLOUR %s", colourStr[colour]);
		drawStringCenter(194, "LOAD PROGRAM ->");
		drawStringCenter(210, "RECOVERY MENU ->");
		drawStringCenter(226, "SHUTDOWN DEVICE");
		drawStringCenter(242, "SUSPEND DEVICE");
		drawStringCenter(258, "REBOOT DEVICE");
		drawStringCenter(274, "RESTART VSH");
		drawStringCenter(290, "EXIT");
		
		switch(selection)
		{
			case 0:
				drawSetColour(WHITE, SKYBLUE);
				if (c_clock == 2)
					drawStringfCenter(130, "CPU CLOCK default");
				else
					drawStringfCenter(130, "CPU CLOCK %d/%d", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency());
				break;
			case 1:
				drawSetColour(WHITE, SKYBLUE);
				if (g_clock == 2)
					drawStringfCenter(146, "GPU CLOCK default");
				else
					drawStringfCenter(146, "GPU CLOCK %d/%d", scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
				break;
			case 2:
				drawSetColour(WHITE, SKYBLUE);
				drawStringfCenter(162, "BATTERY STATUS %s", batteryPercent? "enabled" : "disabled");
				break;
			case 3:
				drawSetColour(WHITE, SKYBLUE);
				drawStringfCenter(178, "VSH MENU COLOUR %s", colourStr[colour]);
				break;
			case 4:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(194, "LOAD PROGRAM ->");
				break;
			case 5:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(210, "RECOVERY MENU ->");
				break;
			case 6:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(226, "SHUTDOWN DEVICE");
				break;
			case 7:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(242, "SUSPEND DEVICE");
				break;
			case 8:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(258, "REBOOT DEVICE");
				break;
			case 9:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(274, "RESTART VSH");
				break;
			case 10:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(290, "EXIT");
				break;
		}
    }
	
	else if (showVSH == 2)
	{
		drawSetColour(WHITE, RGB_GREEN);
		drawStringCenter(100, "PSV VSH MENU");
		
		drawSetColour(WHITE, getColour());
		
		drawStringCenter(130, "<- BACK");
		drawStringfCenter(162, "Settings");
		drawStringfCenter(178, "VitaShell");
		
		switch(selection)
		{
			case 0:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(130, "<- BACK");
				break;
			case 1:
				drawSetColour(WHITE, SKYBLUE);
				drawStringfCenter(162, "Settings");
				break;
			case 2:
				drawSetColour(WHITE, SKYBLUE);
				drawStringfCenter(178, "VitaShell");
				break;
		}
	}
	
    return TAI_CONTINUE(SceInt, ref_hook0, pParam, sync);
}

SceInt checkButtons(SceInt port, tai_hook_ref_t ref_hook, SceCtrlData * ctrl, SceInt count) 
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
			
			if (selection == MAIN_MAX_ITEMS + 1)
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
					saveClockConfig(c_clock, g_clock);
					loadConfig();
				}
				else if ((pressed_buttons & SCE_CTRL_RIGHT) && (c_clock < 3))
				{
					c_clock++;
					scePowerSetArmClockFrequency(profiles[c_clock][0]);
					scePowerSetBusClockFrequency(profiles[c_clock][1]);
					saveClockConfig(c_clock, g_clock);
					loadConfig();
				}
			}
			else if (selection == 1)
			{
				if ((pressed_buttons & SCE_CTRL_LEFT) && (g_clock > 0))
				{
					g_clock--;
					scePowerSetGpuClockFrequency(profiles[g_clock][2]);
					scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
					saveClockConfig(c_clock, g_clock);
					loadConfig();
				}
				else if ((pressed_buttons & SCE_CTRL_RIGHT) && (g_clock < 3))
				{
					g_clock++;
					scePowerSetGpuClockFrequency(profiles[g_clock][2]);
					scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
					saveClockConfig(c_clock, g_clock);
					loadConfig();
				}
			}
			else if (selection == 2)
			{
				if ((pressed_buttons & SCE_CTRL_LEFT) || (pressed_buttons & SCE_CTRL_RIGHT))
				{
					if (!batteryPercent)
						batteryPercent = SCE_TRUE;
					else 
						batteryPercent = SCE_FALSE;
					
					saveMenuConfig(batteryPercent, colour);
					loadConfig();
				}
			}
			else if (selection == 3)
			{
				if (pressed_buttons & SCE_CTRL_LEFT)
				{
					colour--;
					saveMenuConfig(batteryPercent, colour);
					loadConfig();
				}
				else if (pressed_buttons & SCE_CTRL_RIGHT)
				{
					colour++;
					saveMenuConfig(batteryPercent, colour);
					loadConfig();
				}
				
				if (colour > 8)
					colour = 0;
				else if (colour < 0)
					colour = 8;
			}
			else if ((selection == 4) && (pressed_buttons & SCE_CTRL_CROSS))
			{
				selection = 0;
				showVSH = 2;
			}
			else if ((selection == 5) && (pressed_buttons & SCE_CTRL_CROSS)) 
				launchAppByUriExit("PSVVSHREC");
			else if ((selection == 6) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestStandby();
			else if ((selection == 7) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestSuspend();
			else if ((selection == 8) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestColdReset();
			else if ((selection == 9) && (pressed_buttons & SCE_CTRL_CROSS))
				restartVSH();
			else if ((selection == 10) && (pressed_buttons & SCE_CTRL_CROSS))
			{
				selection = 0;
				showVSH = 0;
			}
			
			old_buttons = ctrl->buttons;
			ctrl->buttons = 0; // Disable controls
		}
		else if (showVSH == 2)
		{
			if (pressed_buttons & SCE_CTRL_DOWN)
				selection += 1;
			else if (pressed_buttons & SCE_CTRL_UP)
				selection -= 1;
			
			if (selection == APP_MAX_ITEMS + 1)
				selection = 0;
			if (selection == -1)
				selection = APP_MAX_ITEMS;
			
			if ((selection == 0) && (pressed_buttons & SCE_CTRL_CROSS))
			{
				selection = 0;
				showVSH = 1;
			}
			else if ((selection == 1) && (pressed_buttons & SCE_CTRL_CROSS)) 
				sceAppMgrLaunchAppByUri(0xFFFFF, "settings_dlg:");
			else if ((selection == 2) && (pressed_buttons & SCE_CTRL_CROSS)) 
				launchAppByUriExit("VITASHELL");
			
			old_buttons = ctrl->buttons;
			ctrl->buttons = 0; // Disable controls
		}
		else
		{
			if ((ctrl->buttons & SCE_CTRL_LTRIGGER) && (ctrl->buttons & SCE_CTRL_RTRIGGER) && (ctrl->buttons & SCE_CTRL_START))
			{	
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
				
				showVSH = 1;
			}
		}
	}
  
	return ret;
}

SceInt scePowerSetClockFrequency_patched(tai_hook_ref_t ref_hook, SceInt port, SceInt freq)
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
    return checkButtons(port, ref_hook1, ctrl, count);
}   

static SceInt keys_patched2(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return checkButtons(port, ref_hook2, ctrl, count);
}   

static SceInt keys_patched3(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return checkButtons(port, ref_hook3, ctrl, count);
}   

static SceInt keys_patched4(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return checkButtons(port, ref_hook4, ctrl, count);
}   

static SceInt power_patched1(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(power_hook1, 0, freq);
}

static SceInt power_patched2(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(power_hook2, 1, freq);
}

static SceInt power_patched3(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(power_hook3, 2, freq);
}

static SceInt power_patched4(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(power_hook4, 3, freq);
}

SceVoid _start() __attribute__ ((weak, alias ("module_start")));
SceInt module_start(SceSize argc, const SceVoid * args) 
{	
	if (!(dirExists("ux0:/data/vsh")))
		makeDir("ux0:/data/vsh");
	
	if (!(dirExists("ux0:/data/vsh/titles")))
		makeDir("ux0:/data/vsh/titles");
	
	sceAppMgrAppParamGetString(0, 12, titleID , 256);
	
	loadConfig();

	g_hooks[0] = taiHookFunctionImport(&ref_hook0, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0x7A410B64, // sceDisplaySetFrameBuf
										sceDisplaySetFrameBuf_patched);
	g_hooks[1] = taiHookFunctionImport(&ref_hook1, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0xA9C3CED6, // sceCtrlPeekBufferPositive
										keys_patched1);

	g_hooks[2] = taiHookFunctionImport(&ref_hook2, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0x15F81E8C, // sceCtrlPeekBufferPositive2
										keys_patched2);

	g_hooks[3] = taiHookFunctionImport(&ref_hook3, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0x67E7AB83, // sceCtrlReadBufferPositive
										keys_patched3);

	g_hooks[4] = taiHookFunctionImport(&ref_hook4, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0xC4226A3E, // sceCtrlReadBufferPositive2
										keys_patched4);
	g_hooks[5] = taiHookFunctionImport(&power_hook1, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0x74DB5AE5, // scePowerGetArmClockFrequency
										power_patched1);

	g_hooks[6] = taiHookFunctionImport(&power_hook2, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0xB8D7B3FB, // scePowerSetBusClockFrequency
										power_patched2);

	g_hooks[7] = taiHookFunctionImport(&power_hook3, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0x717DB06C, // scePowerSetGpuClockFrequency
										power_patched3);

	g_hooks[8] = taiHookFunctionImport(&power_hook4, 
										TAI_MAIN_MODULE,
										TAI_ANY_LIBRARY,
										0xA7739DBE, // scePowerSetGpuXbarClockFrequency
										power_patched4);
										
	scePowerSetArmClockFrequency(profiles[c_clock][0]);
	scePowerSetBusClockFrequency(profiles[c_clock][1]);
	scePowerSetGpuClockFrequency(profiles[g_clock][2]);
	scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
	
	return SCE_KERNEL_START_SUCCESS;
}

SceInt module_stop(SceSize argc, const SceVoid *args) 
{
	// free hooks that didn't fail
	if (g_hooks[0] >= 0) 
		taiHookRelease(g_hooks[0], ref_hook0);
	if (g_hooks[1] >= 0) 
		taiHookRelease(g_hooks[1], ref_hook1);
	if (g_hooks[2] >= 0) 
		taiHookRelease(g_hooks[2], ref_hook2);
	if (g_hooks[3] >= 0) 
		taiHookRelease(g_hooks[3], ref_hook3);
	if (g_hooks[4] >= 0) 
		taiHookRelease(g_hooks[4], ref_hook4);
	if (g_hooks[5] >= 0) 
		taiHookRelease(g_hooks[5], power_hook1);
	if (g_hooks[6] >= 0) 
		taiHookRelease(g_hooks[6], power_hook2);
	if (g_hooks[7] >= 0) 
		taiHookRelease(g_hooks[7], power_hook3);
	if (g_hooks[8] >= 0) 
		taiHookRelease(g_hooks[8], power_hook4);
	
	return SCE_KERNEL_STOP_SUCCESS;
}