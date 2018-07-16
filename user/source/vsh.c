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

static SceUID tai_uid[HOOKS_NUM];
static tai_hook_ref_t hook[HOOKS_NUM];

SceInt showVSH = 0;

static SceInt sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, SceInt sync) 
{
	if (hook[0] == 0)
		return -1;
	
	if (showVSH != 0)
	{
		drawSetFrameBuf(pParam);
		Menu_Display(SCE_FALSE);
	}
	
    return TAI_CONTINUE(SceInt, hook[0], pParam, sync);
}

static SceInt HandleControls(int port, tai_hook_ref_t hook, SceCtrlData *ctrl, int count)
{
	int ret = 0;

	if (hook == 0)
		ret = 1;
	else
	{
    	ret = TAI_CONTINUE(int, hook, port, ctrl, count);
	
		if (showVSH == 0)
		{
			if ((ctrl->buttons & SCE_CTRL_L1) && (ctrl->buttons & SCE_CTRL_R1) && (ctrl->buttons & SCE_CTRL_START))
				showVSH = VSH_MAIN_MENU;
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

int vsh_main_thread(SceSize args, void *argp)
{
	sceAppMgrAppParamGetString(0, 12, titleID , 256); // Get titleID of current running application.
	Config_LoadConfig();

	int button = 0;
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, &button);

	if (button == SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE)
	{
		SCE_CTRL_ENTER = SCE_CTRL_CIRCLE;
		SCE_CTRL_CANCEL = SCE_CTRL_CROSS;
	}
	else
	{
		SCE_CTRL_ENTER = SCE_CTRL_CROSS;
		SCE_CTRL_CANCEL = SCE_CTRL_CIRCLE;
	}

	return sceKernelExitDeleteThread(0);
}

SceVoid _start() __attribute__ ((weak, alias ("module_start")));
SceInt module_start(SceSize argc, const SceVoid *args) 
{
	FS_RecursiveMakeDir("ux0:/data/vsh/titles");

	tai_uid[0] = Utils_TaiHookFunctionImport(&hook[0], 0x4FAACD11, 0x7A410B64, sceDisplaySetFrameBuf_patched);

	tai_uid[1] = Utils_TaiHookFunctionImport(&hook[1], 0xD197E3C7, 0xA9C3CED6, sceCtrlPeekBufferPositive_patched);
	tai_uid[2] = Utils_TaiHookFunctionImport(&hook[2], 0xD197E3C7, 0x15F81E8C, sceCtrlPeekBufferPositive2_patched);
	tai_uid[3] = Utils_TaiHookFunctionImport(&hook[3], 0xD197E3C7, 0x67E7AB83, sceCtrlReadBufferPositive_patched);
	tai_uid[4] = Utils_TaiHookFunctionImport(&hook[4], 0xD197E3C7, 0xC4226A3E, sceCtrlReadBufferPositive2_patched);
	tai_uid[5] = Utils_TaiHookFunctionImport(&hook[5], 0x1082DA7F, 0x74DB5AE5, scePowerGetArmClockFrequency_patched);
	tai_uid[6] = Utils_TaiHookFunctionImport(&hook[6], 0x1082DA7F, 0xB8D7B3FB, scePowerSetBusClockFrequency_patched);
	tai_uid[7] = Utils_TaiHookFunctionImport(&hook[7], 0x1082DA7F, 0x717DB06C, scePowerSetGpuClockFrequency_patched);
	tai_uid[8] = Utils_TaiHookFunctionImport(&hook[8], 0x1082DA7F, 0xA7739DBE, scePowerSetGpuXbarClockFrequency_patched);

	SceUID thid = 0;

	if (R_SUCCEEDED(thid = sceKernelCreateThread("vsh_main_thread", (SceKernelThreadEntry)vsh_main_thread, 0x10000100, 0x10000, 0, 0, NULL)))
		sceKernelStartThread(thid, 0, NULL);

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