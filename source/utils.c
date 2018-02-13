#include <libk/stdio.h>

#include "utils.h"

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000
#endif

#define ALIGN(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

SceUID  Utils_TaiHookFunctionImport(tai_hook_ref_t *p_hook, uint32_t import_func_nid, const void *hook_func)
{
	return taiHookFunctionImport(p_hook, TAI_MAIN_MODULE, TAI_ANY_LIBRARY, import_func_nid, hook_func);
}

SceInt Utils_LaunchAppByUriExit(char *titleid) 
{
	SceInt ret = 0;
	char uri[32];
	
	if (strlen(titleid) == 9) // Game or App titleID
	{
		sprintf(uri, "psgm:play?titleid=%s", titleid);
		
		if (R_FAILED(ret = sceAppMgrLaunchAppByUri(0xFFFFF, uri)))
			return ret;
		
		return 0;
	}
	
	if (R_FAILED(ret = sceAppMgrLaunchAppByUri(0xFFFFF, titleid)))
		return ret;
	
	return 0;
}

SceInt Utils_RestartVSH(SceVoid) 
{	
	SceInt ret = 0;
	char *const argv[] = {"restart", NULL};
	
	if (R_FAILED(ret = sceAppMgrLoadExec("app0:eboot.bin", argv, NULL)))
		return ret;
	
	return 0;
}

SceVoid *Utils_SceMalloc(SceSize size)
{
	if (!size)
		return NULL;

	SceSize inner_size = size + sizeof(SceUID) + sizeof(SceSize);
	SceUID block = sceKernelAllocMemBlock("vsh", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, ALIGN(inner_size, PAGE_SIZE), NULL);
	
	if (block <= 0)
		return NULL;

	SceVoid *base = NULL;
	sceKernelGetMemBlockBase(block, &base);
	
	if (!base)
	{
		sceKernelFreeMemBlock(block);
		return NULL;
	}

	*(SceSize *)base = block;
	*(SceSize *)((SceByte *)base + sizeof(SceUID)) = size;

	SceVoid *user_base = (SceVoid *)((SceByte *)base + sizeof(SceUID) + sizeof(SceSize));

	return user_base;
}

SceVoid Utils_SceFree(SceVoid *mem)
{
	if (!mem)
		return;

	SceByte *inner_mem = (SceByte *)mem - sizeof(SceUID) - sizeof(SceSize);
	SceUID block = 0;
	block = *(SceSize *)inner_mem;
	
	if (block <= 0)
		return;
	
	sceKernelFreeMemBlock(block);
}