#include <libk/stdio.h>

#include "utils.h"

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000
#endif

#define ALIGN(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

SceUID  Utils_TaiHookFunctionImport(tai_hook_ref_t *p_hook, uint32_t import_library_nid, uint32_t import_func_nid, const void *hook_func)
{
	return taiHookFunctionImport(p_hook, TAI_MAIN_MODULE, import_library_nid, import_func_nid, hook_func);
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

	void *ptr = NULL;
	SceUID block = 0;
	
	if (R_SUCCEEDED(block = sceKernelAllocMemBlock("vsh", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, ALIGN(size, 4 * 1024), NULL)))
		sceKernelGetMemBlockBase(block, &ptr);
	
	return ptr;
}

SceVoid Utils_SceFree(SceVoid *mem)
{
	SceUID block = 0;
	
	if (R_SUCCEEDED(block = sceKernelFindMemBlockByAddr(mem, 1)))
		sceKernelFreeMemBlock(block);
}