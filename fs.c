#include "fs.h"

SceInt fileExists(const char * path)
{
	SceUID file = sceIoOpen(path, SCE_O_RDONLY, 0777);
	
	if (file >= 0)
	{
		sceIoClose(file);
		return 1;
	}
	
	return 0;
}

SceInt dirExists(const char * path)
{
	SceUID dir = sceIoDopen(path);
	
	if (dir >= 0)
	{
		sceIoDclose(dir);
		return 1;
	}
	
	return 0;
}

SceInt readFile(char * file, SceVoid * buf, SceInt size) 
{
	SceUID fd = sceIoOpen(file, SCE_O_RDONLY, 0);

	if (fd < 0)
		return fd;

	SceInt read = sceIoRead(fd, buf, size);
	sceIoClose(fd);
	
	return read;
}

SceInt writeFile(char * file, SceVoid * buf, SceInt size) 
{
	SceUID fd = sceIoOpen(file, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
	
	if (fd < 0)
		return fd;

	SceInt written = sceIoWrite(fd, buf, size);
	sceIoClose(fd);
	
	return written;
}

SceInt removeFile(char *file)
{
	return sceIoRemove(file);
}