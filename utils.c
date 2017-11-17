#include <libk/stdio.h>

#include "draw.h"
#include "fs.h"
#include "utils.h"

const char * configFile =
	"display_battery = %d\n"
	"bar_colour = %d\n";

SceInt launchAppByUriExit(char * titleid) 
{
	char uri[32];
	sprintf(uri, "psgm:play?titleid=%s", titleid);

	//sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);
	//sceKernelDelayThread(10000);
	sceAppMgrLaunchAppByUri(0xFFFFF, uri);

	return 0;
}

SceVoid restartVSH(SceVoid) 
{	
	char * const argv[] = {"restart", NULL};
	sceAppMgrLoadExec("app0:eboot.bin", argv, NULL);
}

SceInt saveConfig(SceBool batteryPercent, int colour)
{
	SceInt ret = 0;
	
	char buf[512];
	snprintf(buf, 512, configFile, batteryPercent, colour);
	
	if (R_FAILED(ret = writeFile("ux0:/data/vsh/config.cfg", buf, 512)))
		return ret;
	
	return 0;
}

SceInt loadConfig(SceVoid)
{
	SceInt ret = 0;
	
	if (!(fileExists("ux0:/data/vsh/config.cfg")))
	{
		// set these to the following by default:
		batteryPercent = 0;
		colour = 0;
		return saveConfig(batteryPercent, colour);
	}
	
	char buf[512];
	
	if (R_FAILED(ret = readFile("ux0:/data/vsh/config.cfg", buf, 512)))
		return ret;
	
	sscanf(buf, configFile, &batteryPercent, &colour);
	
	return 0;
}

int getColour()
{
	int col = 0;
	
	switch (colour)
	{
		case 0:
			col = RGB_RED;
			break;
		case 1:
			col = RGB_GREEN;
			break;
		case 2:
			col = RGB_BLUE;
			break;
		case 3:
			col = RGB_YELLOW;
			break;
		case 4:
			col = RGB_PURPLE;
			break;
		case 5:
			col = RGB_BLACK;
			break;
		case 6:
			col = RGB_WHITE;
			break;
	}
	
	return col;
}