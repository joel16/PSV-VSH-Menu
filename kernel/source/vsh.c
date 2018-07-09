#include <psp2kern/io/fcntl.h>
#include <psp2kern/io/stat.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <taihen.h>

#define HOOKS_NUM 3
static SceUID hooks[HOOKS_NUM];
static tai_hook_ref_t ref_hooks[HOOKS_NUM];

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res)   ((res)>=0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)      ((res)<0)
/// Returns the level of a result code.

typedef struct sceIoOpenOpt
{
	uint32_t unk_0;
	uint32_t unk_4;
} sceIoOpenOpt;

typedef struct sceIoMkdirOpt 
{
	uint32_t unk_0;
	uint32_t unk_4;
} sceIoMkdirOpt;

static SceUID ksceKernelLoadStartModuleForPid_Patched(SceUID pid) 
{
	if (ref_hooks[0] == 0)
		return -1;

	int ret = 0;

	if (R_SUCCEEDED(TAI_CONTINUE(int, ref_hooks[0], pid))) 
	{
		char titleid[32];
		int result = 0;

		if (R_FAILED(ret = ksceKernelGetProcessTitleId(pid, titleid, 32)))
			return ret;
		
		ksceKernelLoadStartModuleForPid(pid, "ux0:/tai/vsh.suprx", 0, NULL, 0, NULL, &result);
	}
	
	return ret;
}

static int ksceIoOpen_Patched(SceUID pid, const char *file, int flags, SceMode mode, sceIoOpenOpt *opt) 
{
	if (ref_hooks[1] == 0)
		return -1;

	int ret = 0, state = 0;

	ENTER_SYSCALL(state);
	
	if (R_FAILED(ret = TAI_CONTINUE(int, ref_hooks[1], pid, file, flags, mode, opt)))
	{
		if (R_FAILED(ret = ksceIoOpen(file, flags, mode)))
			return ret;
	}

	EXIT_SYSCALL(state);
	
	return ret;
}

static int ksceIoMkdir_Patched(const char *dirname, SceIoMode mode, sceIoMkdirOpt opt) 
{
	if (ref_hooks[2] == 0)
		return -1;

	int ret = 0, state = 0;

	ENTER_SYSCALL(state);

	if (R_FAILED(ret = TAI_CONTINUE(int, ref_hooks[2], dirname, mode, opt)))
	{
		char path[128];
		ksceKernelStrncpyUserToKernel(path, (uintptr_t)dirname, sizeof(path));
		
		if (R_FAILED(ret = ksceIoMkdir(path, mode)))
			return ret;
	}
	
	EXIT_SYSCALL(state);

	return ret;
}

void _start() __attribute__ ((weak, alias("module_start")));

int module_start(SceSize args, void *argp) 
{
	if (R_FAILED(hooks[0] = taiHookFunctionExportForKernel(KERNEL_PID, &ref_hooks[0], "SceKernelModulemgr", 0xC445FA63, 0x432DCC7A, ksceKernelLoadStartModuleForPid_Patched)))
		hooks[0] = taiHookFunctionExportForKernel(KERNEL_PID, &ref_hooks[0], "SceKernelModulemgr", 0x92C9FFC2, 0x998C7AE9, ksceKernelLoadStartModuleForPid_Patched);

	hooks[1] = taiHookFunctionExportForKernel(KERNEL_PID, &ref_hooks[1], "SceIofilemgr", 0x40FD29C7, 0xC3D34965, ksceIoOpen_Patched);
	hooks[2] = taiHookFunctionExportForKernel(KERNEL_PID, &ref_hooks[2], "SceIofilemgr", 0xF2FF276E, 0x8F1ACC32, ksceIoMkdir_Patched);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) 
{
	for (int i = 0; i < HOOKS_NUM; ++i)
	{
		if (R_SUCCEEDED(hooks[i]))
			taiHookReleaseForKernel(hooks[i], ref_hooks[i]);
	}

	return SCE_KERNEL_STOP_SUCCESS;
}