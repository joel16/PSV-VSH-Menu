#include <libk/stdio.h>

#include "fs.h"
#include "utils.h"

SceBool FS_FileExists(const char *path)
{
	SceUID file = 0;
	
	if (R_SUCCEEDED(file = sceIoOpen(path, SCE_O_RDONLY, 0777)))
	{
		sceIoClose(file);
		return SCE_TRUE;
	}
	
	return SCE_FALSE;
}

SceInt FS_ReadFile(char *path, SceVoid *buf, SceInt size)
{
	SceUID file = 0;

	if (R_SUCCEEDED(file = sceIoOpen(path, SCE_O_RDONLY, 0)))
	{
		SceInt read = sceIoRead(file, buf, size);
		sceIoClose(file);
		return read;
	}
	
	return file;
}

SceInt FS_WriteFile(char *path, SceVoid *buf, SceInt size)
{	
	SceUID file = 0;
	
	if (R_SUCCEEDED(file = sceIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777)))
	{
		SceInt written = sceIoWrite(file, buf, size);
		sceIoClose(file);
		return written;
	}
		
	return file;
}

SceInt FS_MakeDir(const char *path)
{
	SceInt ret = 0;

	if (R_FAILED(ret = sceIoMkdir(path, 0777)))
		return ret;

	return 0;
}

SceVoid FS_RecursiveMakeDir(const char *path) 
{
	char tmp[256];
	char *p = NULL;
	size_t len;

	snprintf(tmp, sizeof(tmp), "%s", path);
	len = strlen(tmp);

	if (tmp[len - 1] == '/')
		tmp[len - 1] = 0;

	for (p = tmp + 1; *p; p++)
	{
		if (*p == '/') 
		{
			*p = 0;
			FS_MakeDir(tmp);
			*p = '/';
		}
		FS_MakeDir(tmp);
	}
}