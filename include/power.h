#ifndef PSV_VSH_POWER_H
#define PSV_VSH_POWER_H

#include <vitasdk.h>

SceInt Power_DisplayBatteryPercentage(SceVoid);
SceInt Power_DisplayBatteryLifetime(SceVoid);
SceInt Power_DisplayBatteryTemp(SceInt y);

#endif
