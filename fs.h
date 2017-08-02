#ifndef PSV_VSH_FS_H
#define PSV_VSH_FS_H

#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/devctl.h>

#include <string.h>

SceInt fileExists(const char * path);
SceInt dirExists(const char * path);
SceInt readFile(char * file, SceVoid * buf, SceInt size);
SceInt writeFile(char * file, SceVoid * buf, SceInt size);
SceInt removeFile(char * file);

#endif