#include <libk/stdio.h>

#include "utils.h"

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