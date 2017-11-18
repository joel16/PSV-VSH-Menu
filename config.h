#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

SceBool batteryPercent;
int colour;
int c_clock, g_clock;
char titleID[16];

SceInt saveMenuConfig(SceBool batteryPercent, int colour);
SceInt saveClockConfig(int cpuClock, int gpuClock);
SceInt loadConfig(SceVoid);
int getColour();

#endif