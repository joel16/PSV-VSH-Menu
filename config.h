#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

SceBool batteryPercent, batteryLifeTime, batteryTemp, isUx0;
int c_clock, g_clock, colour;
char titleID[16];

SceInt saveMenuConfig(SceBool batteryPercent, SceBool batteryLifeTime, SceBool batteryTemp, int colour);
SceInt saveClockConfig(int cpuClock, int gpuClock);
SceInt loadConfig(SceVoid);
int getColour();

#endif