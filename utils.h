#ifndef PSV_VSH_UTILS_H
#define PSV_VSH_UTILS_H

#include <vitasdk.h>
#include <taihen.h>

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res)   ((res)>=0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)      ((res)<0)
/// Returns the level of a result code.

SceInt SCE_CTRL_ENTER, SCE_CTRL_CANCEL;

SceUID  _taiHookFunctionImport(tai_hook_ref_t * p_hook, uint32_t import_func_nid, const void * hook_func);
SceInt launchAppByUriExit(char * titleid);
SceInt restartVSH(SceVoid);
SceVoid * _malloc(SceSize size);
SceVoid _free(SceVoid * mem);

#endif