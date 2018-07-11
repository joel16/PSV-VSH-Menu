#include <libk/stdio.h>

#include <vitasdk.h>
#include <taihen.h>

#include "config.h"
#include "draw.h"
#include "fs.h"
#include "menus.h"
#include "power.h"
#include "utils.h"

#define HOOKS_NUM 5

static SceUID g_hooks[HOOKS_NUM];
static tai_hook_ref_t ref_hook[HOOKS_NUM];

static SceBool run_thread = SCE_TRUE;

//static SceUID drawMutex = -1;

SceInt sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, SceInt sync) 
{
	if (ref_hook[0] == 0)
		return -1;
	
	if (showVSH != 0)
		drawSetFrameBuf(pParam);
	
    return TAI_CONTINUE(SceInt, ref_hook[0], pParam, sync);
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


int vsh_main_thread(SceSize args, void *argp)
{
	sceKernelDelayThread(10000000);

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

	while(run_thread)
	{
		Menu_HandleControls();
		sceKernelDelayThread(100000);
	}

	return sceKernelExitDeleteThread(0);
}

SceVoid _start() __attribute__ ((weak, alias ("module_start")));
SceInt module_start(SceSize argc, const SceVoid *args) 
{
	FS_RecursiveMakeDir("ux0:/data/vsh/titles");

	g_hooks[0] = Utils_TaiHookFunctionImport(&ref_hook[0], 0x4FAACD11, 0x7A410B64, sceDisplaySetFrameBuf_patched); // sceDisplaySetFrameBuf
	g_hooks[1] = Utils_TaiHookFunctionImport(&ref_hook[1], 0x1082DA7F, 0x74DB5AE5, power_patched1);                // scePowerGetArmClockFrequency
	g_hooks[2] = Utils_TaiHookFunctionImport(&ref_hook[2], 0x1082DA7F, 0xB8D7B3FB, power_patched2);                // scePowerSetBusClockFrequency
	g_hooks[3] = Utils_TaiHookFunctionImport(&ref_hook[3], 0x1082DA7F, 0x717DB06C, power_patched3);                // scePowerSetGpuClockFrequency
	g_hooks[4] = Utils_TaiHookFunctionImport(&ref_hook[4], 0x1082DA7F, 0xA7739DBE, power_patched4);                // scePowerSetGpuXbarClockFrequency

	run_thread = SCE_TRUE;
	SceUID thid = 0;

	if (R_SUCCEEDED(thid = sceKernelCreateThread("vsh_main_thread", (SceKernelThreadEntry)vsh_main_thread, 0x10000100, 0x10000, 0, 0, NULL)))
		sceKernelStartThread(thid, 0, NULL);

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