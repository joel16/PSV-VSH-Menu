#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

SceBool batteryPercent;
int colour;

SceInt saveConfig(SceBool batteryPercent, int colour);
SceInt loadConfig(SceVoid);
int getColour();

#endif