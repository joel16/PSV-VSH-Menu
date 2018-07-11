#include "threads.h"
#include "utils.h"

volatile SceBool paused = SCE_FALSE;
static SceUID main_thid = 0;
static int main_prio = 0;

static int Thread_PauseThread(SceSize args, void *argp)
{
    while(1)
    {
        if (!paused)
            break;
    }

    return sceKernelExitDeleteThread(0);
}

SceVoid Thread_PauseMainThread(SceVoid)
{
    SceKernelThreadInfo info;
    info.size = sizeof(SceKernelThreadInfo);

    if (main_thid == 0)
    {
        SceUID thid = 0x40010001;
        SceUID curr = sceKernelGetThreadId();
        
        for(; thid <= 0x40010FFF; ++thid)
        {
            if (thid == curr)
                continue;
            
            int ret = sceKernelGetThreadInfo(thid, &info);

            if (ret < 0)
                continue;

            main_thid = thid;
            main_prio = info.currentPriority;
            break;
        }
        if (main_thid == 0)
            return;
    }
    else
    {
        int ret = sceKernelGetThreadInfo(main_thid, &info);
        if (ret >= 0)
            main_prio = info.currentPriority;
        else
            main_prio = 0x10000100;
    }
    
    sceKernelChangeThreadPriority(main_thid, 0);

    while(1)
    {
        paused = SCE_TRUE;
        SceUID thid = 0;

        if (R_SUCCEEDED(thid = sceKernelCreateThread("vsh_pause_thread", (SceKernelThreadEntry)Thread_PauseThread, 0x40, 0x4000, 0, SCE_KERNEL_CPU_MASK_USER_0, NULL)))
            sceKernelStartThread(thid, 0, NULL);

        sceKernelGetThreadInfo(main_thid, &info);
        
        if (info.status != SCE_THREAD_RUNNING)
            break;
        
        paused = SCE_FALSE;
        sceKernelDelayThread(1000);
    }
}

SceVoid Thread_ResumeMainThread(SceVoid)
{
    if (main_thid == 0)
        return;
    
    if (!paused)
        return;

    paused = SCE_FALSE;
    sceKernelChangeThreadPriority(main_thid, main_prio);
}
