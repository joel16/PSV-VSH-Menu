#ifndef PSV_VSH_UTILS_H
#define PSV_VSH_UTILS_H

#include <vitasdk.h>
#include <taihen.h>

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res)   ((res)>=0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)      ((res)<0)
/// Returns the level of a result code.

#define SCE_KERNEL_CPU_MASK_SHIFT   (16)
#define SCE_KERNEL_CPU_MASK_USER_0  (0x01 << SCE_KERNEL_CPU_MASK_SHIFT)
#define SCE_KERNEL_CPU_MASK_USER_1  (0x02 << SCE_KERNEL_CPU_MASK_SHIFT)
#define SCE_KERNEL_CPU_MASK_USER_2  (0x04 << SCE_KERNEL_CPU_MASK_SHIFT)

SceUID  Utils_TaiHookFunctionImport(tai_hook_ref_t *p_hook, uint32_t import_library_nid, uint32_t import_func_nid, const void *hook_func);
SceInt Utils_LaunchAppByUriExit(char *titleid);
SceInt Utils_RestartVSH(SceVoid);
SceVoid *Utils_SceMalloc(SceSize size);
SceVoid Utils_SceFree(SceVoid *mem);
void Utils_SetMax(SceInt *set, SceInt value, SceInt max);
void Utils_SetMin(SceInt *set, SceInt value, SceInt min);

#endif
