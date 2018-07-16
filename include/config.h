#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

SceBool batteryPercent, batteryLifeTime, batteryTemp, batteryDisplay;
int c_clock, g_clock, colour;
char titleID[16];
char app_title[5][50], app_titleID[5][50];

SceInt Config_SaveMenuConfig(SceBool, SceBool, SceBool, SceBool, SceInt);
SceInt Config_SaveClockConfig(int cpuClock, int gpuClock);
SceInt Config_LoadConfig(SceVoid);
int Config_GetVSHColour();

#endif