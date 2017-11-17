#include "fs.h"
#include "utils.h"

SceBool fileExists(const char * path)
{
	SceUID file = 0;
	
	if (R_SUCCEEDED(file = sceIoOpen(path, SCE_O_RDONLY, 0777)))
	{
		sceIoClose(file);
		return SCE_TRUE;
	}
	
	return SCE_FALSE;
}

SceBool dirExists(const char * path)
{	
	SceUID dir = 0;
	
	if (R_SUCCEEDED(dir = sceIoDopen(path)))
	{
		sceIoDclose(dir);
		return SCE_TRUE;
	}
	
	return SCE_FALSE;
}

SceInt readFile(char * path, SceVoid * buf, SceInt size) 
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

SceInt writeFile(char * path, SceVoid * buf, SceInt size) 
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

SceInt makeDir(char * path)
{
	return sceIoMkdir(path, 0777);
}

SceInt removeFile(char * path)
{
	return sceIoRemove(path);
}