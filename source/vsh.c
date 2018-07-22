#include <libk/stdio.h>

#include <vitasdk.h>
#include <taihen.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "menus.h"
#include "power.h"
#include "utils.h"

#define HOOKS_NUM 9

#define CLOCK_SET_DELAY_INIT      5000000 // 5 seconds
#define CLOCK_SET_DELAY_INTERVAL 30000000 // 30 seconds

#define BUTTON_COMBO_1 ((ctrl->buttons & SCE_CTRL_LTRIGGER) && (ctrl->buttons & SCE_CTRL_RTRIGGER) && (ctrl->buttons & SCE_CTRL_START))
#define BUTTON_COMBO_2 ((ctrl->buttons & SCE_CTRL_L1) && (ctrl->buttons & SCE_CTRL_R1) && (ctrl->buttons & SCE_CTRL_START))
#define BUTTON_COMBO_3 ((ctrl->buttons & SCE_CTRL_LTRIGGER) && (ctrl->buttons & SCE_CTRL_RTRIGGER) && (ctrl->buttons & SCE_CTRL_SELECT))
#define BUTTON_COMBO_4 ((ctrl->buttons & SCE_CTRL_L1) && (ctrl->buttons & SCE_CTRL_R1) && (ctrl->buttons & SCE_CTRL_SELECT))

static SceUID tai_uid[HOOKS_NUM];
static tai_hook_ref_t hook[HOOKS_NUM];

SceInt showVSH = 0;
static SceBool isConfigSet = SCE_FALSE;
static SceUInt64 timer = 0;

SceInt sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, SceDisplaySetBufSync sync) 
{
	if (hook[0] == 0)
		return -1;
	
	drawSetFrameBuf(pParam);

	if (timer == 0)
		timer = sceKernelGetProcessTimeWide();
	else if ((sceKernelGetProcessTimeWide() - timer) > (isConfigSet? CLOCK_SET_DELAY_INTERVAL : CLOCK_SET_DELAY_INIT)) // Check in 5 seconds initially
	{
		// if current clock state don't match the ones in config -> re set the desired clock config.
		if ((scePowerGetArmClockFrequency() != profiles[c_clock][0]) || (scePowerGetBusClockFrequency() != profiles[c_clock][1]) || 
			(scePowerGetGpuClockFrequency() != profiles[g_clock][2]) || (scePowerGetGpuXbarClockFrequency() != profiles[g_clock][3]))
		{
			scePowerSetArmClockFrequency(profiles[c_clock][0]);
			scePowerSetBusClockFrequency(profiles[c_clock][1]);
			scePowerSetGpuClockFrequency(profiles[g_clock][2]);
			scePowerSetGpuXbarClockFrequency(profiles[g_clock][3]);
			timer = 0;
			isConfigSet = SCE_TRUE; // Once this is true check if the clock states have changed in 30 second intervals
		}
	}

	drawSetColour(WHITE, Config_GetVSHColour());
	drawStringf(0, 0, "CPU: %d/%d MHz", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency());
	drawStringf(0, 16, "GPU: %d/%d MHz", scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());

	if ((batteryDisplay) && (showVSH == 0))
	{
		if (batteryPercent)
			Power_DisplayBatteryPercentage();
		if (batteryLifeTime)
			Power_DisplayBatteryLifetime();
		if (batteryTemp)
			Power_DisplayBatteryTemp();
	}

	if (showVSH != 0)
	{
		if (batteryPercent)
			Power_DisplayBatteryPercentage();
		if (batteryLifeTime)
			Power_DisplayBatteryLifetime();
		if (batteryTemp)
			Power_DisplayBatteryTemp();
		
		Menu_Display(SCE_FALSE);
	}

	return TAI_CONTINUE(SceInt, hook[0], pParam, sync);
}

static SceInt HandleControls(int port, tai_hook_ref_t hook, SceCtrlData *ctrl, int count)
{
	SceInt ret = 0;

	if (hook == 0)
		ret = 1;
	else
	{
    	ret = TAI_CONTINUE(int, hook, port, ctrl, count);
	
		if (showVSH == 0)
		{
			if (BUTTON_COMBO_1 || BUTTON_COMBO_2 || BUTTON_COMBO_3 || BUTTON_COMBO_4)
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
				
				showVSH = VSH_MAIN_MENU;
			}
		}
		else if (showVSH != 0)
		{
			pressed_buttons = ctrl->buttons & ~old_buttons;

			Menu_HandleControls(pressed_buttons);

			old_buttons = ctrl->buttons;
			ctrl->buttons = 0;
		}
	}

	return ret;
}

static SceInt sceCtrlPeekBufferPositive_patched(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
	return HandleControls(port, hook[1], ctrl, count);
}   

static SceInt sceCtrlPeekBufferPositive2_patched(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
	return HandleControls(port, hook[2], ctrl, count);
}   

static SceInt sceCtrlReadBufferPositive_patched(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return HandleControls(port, hook[3], ctrl, count);
}   

static SceInt sceCtrlReadBufferPositive2_patched(SceInt port, SceCtrlData *ctrl, SceInt count) 
{
    return HandleControls(port, hook[4], ctrl, count);
} 

static SceInt scePowerSetClockFrequency_patched(tai_hook_ref_t hook, SceInt port, SceInt freq)
{
	if (c_clock == -1)
		return TAI_CONTINUE(SceInt, hook, freq);
	else
		return TAI_CONTINUE(SceInt, hook, profiles[c_clock][port]);
	
	if (g_clock == -1)
		return TAI_CONTINUE(SceInt, hook, freq);
	else
		return TAI_CONTINUE(SceInt, hook, profiles[g_clock][port]);
}

static SceInt scePowerGetArmClockFrequency_patched(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(hook[5], 0, freq);
}

static SceInt scePowerSetBusClockFrequency_patched(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(hook[6], 1, freq);
}

static SceInt scePowerSetGpuClockFrequency_patched(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(hook[7], 2, freq);
}

static SceInt scePowerSetGpuXbarClockFrequency_patched(SceInt freq) 
{
    return scePowerSetClockFrequency_patched(hook[8], 3, freq);
}

SceVoid _start() __attribute__ ((weak, alias ("module_start")));
SceInt module_start(SceSize argc, const SceVoid *args) 
{
	sceAppMgrAppParamGetString(0, 12, titleID , 256); // Get titleID of current running application.
	FS_RecursiveMakeDir("ur0:/data/vsh/titles");
	Config_LoadConfig();

	tai_uid[0] = Utils_TaiHookFunctionImport(&hook[0], 0x4FAACD11, 0x7A410B64, sceDisplaySetFrameBuf_patched);
	tai_uid[1] = Utils_TaiHookFunctionImport(&hook[1], 0xD197E3C7, 0xA9C3CED6, sceCtrlPeekBufferPositive_patched);
	tai_uid[2] = Utils_TaiHookFunctionImport(&hook[2], 0xD197E3C7, 0x15F81E8C, sceCtrlPeekBufferPositive2_patched);
	tai_uid[3] = Utils_TaiHookFunctionImport(&hook[3], 0xD197E3C7, 0x67E7AB83, sceCtrlReadBufferPositive_patched);
	tai_uid[4] = Utils_TaiHookFunctionImport(&hook[4], 0xD197E3C7, 0xC4226A3E, sceCtrlReadBufferPositive2_patched);
	tai_uid[5] = Utils_TaiHookFunctionImport(&hook[5], 0x1082DA7F, 0x74DB5AE5, scePowerGetArmClockFrequency_patched);
	tai_uid[6] = Utils_TaiHookFunctionImport(&hook[6], 0x1082DA7F, 0xB8D7B3FB, scePowerSetBusClockFrequency_patched);
	tai_uid[7] = Utils_TaiHookFunctionImport(&hook[7], 0x1082DA7F, 0x717DB06C, scePowerSetGpuClockFrequency_patched);
	tai_uid[8] = Utils_TaiHookFunctionImport(&hook[8], 0x1082DA7F, 0xA7739DBE, scePowerSetGpuXbarClockFrequency_patched);
	
	return SCE_KERNEL_START_SUCCESS;
}

SceInt module_stop(SceSize argc, const SceVoid *args) 
{
	// free hooks that didn't fail
	for (SceInt i = 0; i < HOOKS_NUM; i++)
	{
		if (R_SUCCEEDED(tai_uid[i])) 
			taiHookRelease(tai_uid[i], hook[i]);
	}

	return SCE_KERNEL_STOP_SUCCESS;
}