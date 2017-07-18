#include <psp2/appmgr.h>
#include <psp2/ctrl.h>
#include <psp2/display.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/power.h>
#include <taihen.h>

#include <stdio.h>
#include <string.h>

#include "draw.h"
#include "fs.h"

static int showVSH = 0;
static int selection = 0;
static int colour = 0;
static uint64_t c_clock = 0, g_clock = 0;

static uint32_t old_buttons, pressed_buttons;

static SceUID g_hooks[11];

static tai_hook_ref_t ref_hook0, ref_hook1, ref_hook2, ref_hook3, ref_hook4;
static tai_hook_ref_t power_hook1, power_hook2, power_hook3, power_hook4;

static int profile_max_battery[] = {111, 111, 111, 111};
static int profile_default[] = {266, 166, 166, 111};
static int profile_max_performance[] = {444, 222, 222, 166};
static int profile_game[] = {444, 222, 222, 166};

static int* profiles[4] = {profile_max_battery, profile_default, profile_game, profile_max_performance};

int launchAppByUriExit(char * titleid) 
{
	char uri[32];
	sprintf(uri, "psgm:play?titleid=%s", titleid);

	//sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);
	//sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);

	return 0;
}

static void restartVSH(void) 
{	
	char * const argv[] = {"restart", NULL};
	sceAppMgrLoadExec("app0:eboot.bin", argv, NULL);
}

int sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, int sync) 
{
    drawSetFrameBuf(pParam);
    
	if(showVSH == 1)
	{
		drawSetColour(WHITE, RGB_GREEN);
		drawStringCenter(100, "PSV VSH MENU");
		
		drawSetColour(WHITE, RGB_RED);
		
		if (c_clock == 2)
			drawStringfCenter(130, "CPU CLOCK default");
		else 
			drawStringfCenter(130, "CPU CLOCK %d/%d", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency());
		if (g_clock == 2)
			drawStringfCenter(146, "GPU CLOCK default");
		else
			drawStringfCenter(146, "GPU CLOCK %d/%d", scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
		
		//drawStringfCenter(162, "VSH MENU COLOUR %s", readVSHColours());
		drawStringCenter(162, "RECOVERY MENU ->");
		drawStringCenter(178, "LOAD PROGRAM ->");
		drawStringCenter(194, "SHUTDOWN DEVICE");
		drawStringCenter(210, "SUSPEND DEVICE");
		drawStringCenter(226, "REBOOT DEVICE");
		drawStringCenter(242, "RESTART VSH");
		
		drawStringCenter(258, "EXIT");
		
		switch(selection)
		{
			case 0:
				drawSetColour(WHITE, SKYBLUE);
				if	(c_clock == 2)
					drawStringfCenter(130, "CPU CLOCK default");
				else
					drawStringfCenter(130, "CPU CLOCK %d/%d", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency());
				break;
			case 1:
				drawSetColour(WHITE, SKYBLUE);
				if	(g_clock == 2)
					drawStringfCenter(146, "GPU CLOCK default");
				else
					drawStringfCenter(146, "GPU CLOCK %d/%d", scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
				break;
			/*case 2:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(162, "VSH MENU COLOUR");
				break;*/
			case 2:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(162, "RECOVERY MENU ->");
				break;
			case 3:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(178, "LOAD PROGRAM ->");
				break;
			case 4:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(194, "SHUTDOWN DEVICE");
				break;
			case 5:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(210, "SUSPEND DEVICE");
				break;
			case 6:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(226, "REBOOT DEVICE");
				break;
			case 7:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(242, "RESTART VSH");
				break;
			case 8:
				drawSetColour(WHITE, SKYBLUE);
				drawStringCenter(258, "EXIT");
				break;
		}
    }
	
    return TAI_CONTINUE(int, ref_hook0, pParam, sync);
}   

int checkButtons(int port, tai_hook_ref_t ref_hook, SceCtrlData *ctrl, int count) 
{
	int ret;

	if (ref_hook == 0)
		ret = 1;
	
	else
	{
		ret = TAI_CONTINUE(int, ref_hook, port, ctrl, count);
		
		pressed_buttons = ctrl->buttons & ~old_buttons;

		if ((ctrl->buttons & SCE_CTRL_LTRIGGER) && (ctrl->buttons & SCE_CTRL_RTRIGGER) && (ctrl->buttons & SCE_CTRL_START)) {
			showVSH = 1;
		}

		if(showVSH == 1) // Main VSH Menu
		{
			if (pressed_buttons & SCE_CTRL_DOWN)
				selection += 1;
			else if (pressed_buttons & SCE_CTRL_UP)
				selection -= 1;
			
			if (selection == 9)
				selection = 0;
			if (selection == -1)
				selection = 8;
			
			if (selection == 0) 
			{
				if ((pressed_buttons & SCE_CTRL_LEFT) && (c_clock > 0))
				{
					c_clock--;
					scePowerSetArmClockFrequency(profiles[c_clock][0]);
					scePowerSetBusClockFrequency(profiles[c_clock][1]);
				}
				else if ((pressed_buttons & SCE_CTRL_RIGHT) && (c_clock < 3))
				{
					c_clock++;
					scePowerSetArmClockFrequency(profiles[c_clock][0]);
					scePowerSetBusClockFrequency(profiles[c_clock][1]);
				}
			}
			else if (selection == 1)
			{
				if ((pressed_buttons & SCE_CTRL_LEFT) && (g_clock > 0))
				{
					g_clock--;
					scePowerSetGpuClockFrequency(profiles[g_clock][2]);
					scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
				}
				else if ((pressed_buttons & SCE_CTRL_RIGHT) && (g_clock < 3))
				{
					g_clock++;
					scePowerSetGpuClockFrequency(profiles[g_clock][2]);
					scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
				}
			}
			
			//else if ((selection == 3) && (pressed_buttons & SCE_CTRL_CROSS)) 
				//App launch
			else if ((selection == 4) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestStandby();
			else if ((selection == 5) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestSuspend();
			else if ((selection == 6) && (pressed_buttons & SCE_CTRL_CROSS)) 
				scePowerRequestColdReset();
			else if ((selection == 7) && (pressed_buttons & SCE_CTRL_CROSS))
				restartVSH();
			else if (((selection == 8) && (pressed_buttons & SCE_CTRL_CROSS)) || ((ctrl->buttons & SCE_CTRL_SELECT) && (ctrl->buttons & SCE_CTRL_DOWN)))
				showVSH = 0;
			
			old_buttons = ctrl->buttons;
			ctrl->buttons = 0; // Disable controls
		}
		else
		{
			if ((ctrl->buttons & SCE_CTRL_SELECT) && (ctrl->buttons & SCE_CTRL_UP))
			{
				if (!(dirExists("ux0:/data/uvsh")))
				{
					char buf[2];
					
					SceUID dir = sceIoDopen("ux0:/data/uvsh");
					sceIoMkdir("ux0:/data/uvsh", 0777);
					sceIoDclose(dir);
					
					if (!(fileExists("ux0:/data/uvsh/colours.bin")))
					{
						sprintf(buf, "%d", colour);
						writeFile("ux0:/data/uvsh/colours.bin", buf, 2);
					}
				}
				
				if(c_clock == -1)
				{
					profile_game[0] = scePowerGetArmClockFrequency();
					profile_game[1] = scePowerGetBusClockFrequency();
					c_clock = 0;
				}
				
				if(g_clock == -1)
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

int scePowerSetClockFrequency_patched(tai_hook_ref_t ref_hook, int port, int freq)
{
	if(c_clock == -1)
		return TAI_CONTINUE(int, ref_hook, freq);
	else
		return TAI_CONTINUE(int, ref_hook, profiles[c_clock][port]);
	
	if(g_clock == -1)
		return TAI_CONTINUE(int, ref_hook, freq);
	else
		return TAI_CONTINUE(int, ref_hook, profiles[g_clock][port]);
}

static int keys_patched1(int port, SceCtrlData *ctrl, int count) 
{
    return checkButtons(port, ref_hook1, ctrl, count);
}   

static int keys_patched2(int port, SceCtrlData *ctrl, int count) 
{
    return checkButtons(port, ref_hook2, ctrl, count);
}   

static int keys_patched3(int port, SceCtrlData *ctrl, int count) 
{
    return checkButtons(port, ref_hook3, ctrl, count);
}   

static int keys_patched4(int port, SceCtrlData *ctrl, int count) 
{
    return checkButtons(port, ref_hook4, ctrl, count);
}   

static int power_patched1(int freq) 
{
    return scePowerSetClockFrequency_patched(power_hook1, 0, freq);
}

static int power_patched2(int freq) 
{
    return scePowerSetClockFrequency_patched(power_hook2, 1, freq);
}

static int power_patched3(int freq) 
{
    return scePowerSetClockFrequency_patched(power_hook3, 2, freq);
}

static int power_patched4(int freq) 
{
    return scePowerSetClockFrequency_patched(power_hook4, 3, freq);
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_stop(SceSize argc, const void *args);

int module_start(SceSize argc, const void *args) 
{

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
										
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) 
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