#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

SceBool batteryPercent;
int colour;
int c_clock, g_clock;

SceInt saveConfig(int cpuClock, int gpuClock, SceBool batteryPercent, int colour);
SceInt loadConfig(SceVoid);
int getColour();

#endif