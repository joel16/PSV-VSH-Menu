#include "fs.h"

int fileExists(const char *path)
{
	SceUID file = sceIoOpen(path, SCE_O_RDONLY, 0777);
	
	if (file >= 0)
	{
		sceIoClose(file);
		return 1;
	}
	else
		return 0;
}

int dirExists(const char *path)
{
	SceUID dir = sceIoDopen(path);
	
	if (dir >= 0)
	{
		sceIoDclose(dir);
		return 1;
	}
	else
		return 0;
}

int readFile(char *file, void *buf, int size) 
{
	SceUID fd = sceIoOpen(file, SCE_O_RDONLY, 0);

	if (fd < 0)
		return fd;

	int read = sceIoRead(fd, buf, size);
	sceIoClose(fd);
	
	return read;
}

int writeFile(char *file, void *buf, int size) 
{
	SceUID fd = sceIoOpen(file, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
	
	if (fd < 0)
		return fd;

	int written = sceIoWrite(fd, buf, size);
	sceIoClose(fd);
	
	return written;
}

int removeFile(char *file)
{
	return sceIoRemove(file);
}