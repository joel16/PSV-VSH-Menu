#ifndef PSV_VSH_UTILS_H
#define PSV_VSH_UTILS_H

#include <vitasdk.h>

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res)   ((res)>=0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)      ((res)<0)
/// Returns the level of a result code.

SceInt launchAppByUriExit(char * titleid);
SceVoid restartVSH(SceVoid);

#endif