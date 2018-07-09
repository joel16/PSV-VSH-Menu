#include "config.h"
#include "draw.h"
#include "power.h"
#include "utils.h"

SceInt Power_DisplayBatteryPercentage(SceVoid)
{
	SceInt ret = 0;
	
	if (R_FAILED(ret = scePowerGetBatteryLifePercent()))
		return ret;
		
	drawSetColour(WHITE, Config_GetVSHColour());
	
	if (ret == 100)
		drawStringf(880, 0, "%d %%", ret);
	else
		drawStringf(896, 0, "%d %%", ret);
	
	return 0;
}

SceInt Power_DisplayBatteryLifetime(SceVoid)
{
	SceInt ret = 0;
	
	if (R_FAILED(ret = scePowerGetBatteryLifeTime()))
		return ret;
	
	drawSetColour(WHITE, Config_GetVSHColour());
	drawStringfCenter(0, "%02ih %02im", ret / 60, ret - (ret / 60 * 60));
	
	return 0;
}

SceInt Power_DisplayBatteryTemp(SceVoid)
{
	SceInt ret = 0;
	
	if (R_FAILED(ret = scePowerGetBatteryTemp()))
		return ret;
	
	drawSetColour(WHITE, Config_GetVSHColour());
	drawStringf(0, 0, "Temp: %02i C", ret / 100);
	
	return 0;
}