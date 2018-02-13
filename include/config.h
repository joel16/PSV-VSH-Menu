#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

SceBool batteryPercent, batteryLifeTime, batteryTemp;
int c_clock, g_clock, colour;
char titleID[16];
char app_title[10][50], app_titleID[10][50];

SceInt Config_SaveMenuConfig(SceBool batteryPercent, SceBool batteryLifeTime, SceBool batteryTemp, int colour);
SceInt Config_SaveClockConfig(int cpuClock, int gpuClock);
SceInt Config_LoadConfig(SceVoid);
int Config_GetVSHColour();

#endif