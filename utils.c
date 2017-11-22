#include <libk/stdio.h>

#include "utils.h"

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000
#endif

#define ALIGN(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

SceUID  _taiHookFunctionImport(tai_hook_ref_t * p_hook, uint32_t import_func_nid, const void * hook_func)
{
	return taiHookFunctionImport(p_hook, TAI_MAIN_MODULE, TAI_ANY_LIBRARY, import_func_nid, hook_func);
}

SceInt launchAppByUriExit(char * titleid) 
{
	char uri[32];
	sprintf(uri, "psgm:play?titleid=%s", titleid);

	//sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);
	//sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);

	return 0;
}

SceVoid restartVSH(SceVoid) 
{	
	char * const argv[] = {"restart", NULL};
	sceAppMgrLoadExec("app0:eboot.bin", argv, NULL);
}

SceVoid * _malloc(SceSize size)
{
	if (!size)
		return NULL;

	SceSize inner_size = size + sizeof(SceUID) + sizeof(SceSize);
	SceUID block = sceKernelAllocMemBlock("vsh", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, ALIGN(inner_size, PAGE_SIZE), NULL);
	
	if (block <= 0)
		return NULL;

	SceVoid * base = NULL;
	sceKernelGetMemBlockBase(block, &base);
	
	if (!base)
	{
		sceKernelFreeMemBlock(block);
		return NULL;
	}

	*(SceSize *)base = block;
	*(SceSize *)((SceByte *)base + sizeof(SceUID)) = size;

	SceVoid * user_base = (SceVoid *)((SceByte *)base + sizeof(SceUID) + sizeof(SceSize));

	return user_base;
}

SceVoid _free(SceVoid * mem)
{
	if (!mem)
		return;

	SceByte * inner_mem = (SceByte *)mem - sizeof(SceUID) - sizeof(SceSize);
	SceUID block = 0;
	block = *(SceSize *)inner_mem;
	
	if (block <= 0)
		return;
	
	sceKernelFreeMemBlock(block);
}

SceInt initAppUtil(SceVoid)
{
	SceAppUtilInitParam init;
	SceAppUtilBootParam boot;
	memset(&init, 0, sizeof(SceAppUtilInitParam));
	memset(&boot, 0, sizeof(SceAppUtilBootParam));
	
	SceInt ret = 0;
	
	if (R_FAILED(ret = sceAppUtilInit(&init, &boot)))
		return ret;
	
	return 0;
}

SceInt termAppUtil(SceVoid)
{
	SceInt ret = 0;
	
	if (R_FAILED(ret = sceAppUtilShutdown()))
		return ret;
	
	return 0;
}

SceVoid getCtrlButtons(SceVoid)
{
	int enter_button = 0;
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, &enter_button);
	
	SCE_CTRL_ENTER = SCE_CTRL_CROSS;
	SCE_CTRL_CANCEL = SCE_CTRL_CIRCLE;
	
	if (enter_button == SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE) 
	{
		SCE_CTRL_ENTER = SCE_CTRL_CIRCLE;
		SCE_CTRL_CANCEL = SCE_CTRL_CROSS;
	}
}