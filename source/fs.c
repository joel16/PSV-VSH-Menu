#include <libk/stdio.h>

#include "fs.h"
#include "utils.h"

#include <kuio.h>

SceBool FS_FileExists(const char *path)
{
	SceUID file = 0;
	kuIoOpen(path, SCE_O_RDONLY, &file);

	if (R_SUCCEEDED(file))
	{
		kuIoClose(file);
		return SCE_TRUE;
	}
	
	return SCE_FALSE;
}

SceInt FS_ReadFile(char *path, SceVoid *buf, SceInt size)
{
	SceUID file = 0;
	kuIoOpen(path, SCE_O_RDONLY, &file);

	if (R_SUCCEEDED(file))
	{
		SceInt read = kuIoRead(file, buf, size);
		kuIoClose(file);
		return read;
	}
	
	return file;
}

SceInt FS_WriteFile(char *path, SceVoid *buf, SceInt size)
{	
	SceUID file = 0;
	kuIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, &file);
	
	if (R_SUCCEEDED(file))
	{
		SceInt written = kuIoWrite(file, buf, size);
		kuIoClose(file);
		return written;
	}
		
	return file;
}

SceInt FS_MakeDir(const char *path)
{
	SceInt ret = 0;

	if (R_FAILED(ret = kuIoMkdir(path)))
		return ret;

	return 0;
}

SceVoid FS_RecursiveMakeDir(const char *path) 
{
	char tmp[256];
	char *p = NULL;
	size_t len;

	sceClibSnprintf(tmp, sizeof(tmp), "%s", path);
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