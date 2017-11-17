#ifndef PSV_VSH_FS_H
#define PSV_VSH_FS_H

#include <vitasdk.h>

SceBool fileExists(const char * path);
SceBool dirExists(const char * path);
SceInt readFile(char * path, SceVoid * buf, SceInt size);
SceInt writeFile(char * path, SceVoid * buf, SceInt size);
SceInt makeDir(char * path);
SceInt removeFile(char * path);

#endif