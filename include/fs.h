#ifndef PSV_VSH_FS_H
#define PSV_VSH_FS_H

#include <vitasdk.h>

SceBool FS_FileExists(const char *path);
SceBool FS_DirExists(const char *path);
SceInt FS_ReadFile(char *path, SceVoid *buf, SceInt size);
SceInt FS_WriteFile(char *path, SceVoid *buf, SceInt size);
SceInt FS_MakeDir(const char *path);
SceVoid FS_RecursiveMakeDir(const char *path);
SceInt FS_RemoveFile(const char *path);

#endif