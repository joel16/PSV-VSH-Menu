#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/devctl.h>

#include <string.h>

int fileExists(const char * path);
int dirExists(const char * path);
int readFile(char * file, void * buf, int size);
int writeFile(char * file, void * buf, int size);
int removeFile(char * file);