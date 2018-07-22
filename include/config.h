#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

SceBool batteryPercent, batteryLifeTime, batteryTemp, batteryDisplay;
SceBool fps, fpsDisplay;
SceInt c_clock, g_clock, colour;
char titleID[16];
char app_title[5][50], app_titleID[5][50];

SceInt Config_SaveMenuConfig(SceBool, SceBool, SceBool, SceBool, SceInt, SceBool, SceBool);
SceInt Config_SaveClockConfig(SceInt cpuClock, SceInt gpuClock);
SceInt Config_LoadConfig(SceVoid);
SceInt Config_GetVSHColour(SceVoid);

#endif