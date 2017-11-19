#include "config.h"
#include "draw.h"
#include "power.h"
#include "utils.h"

SceVoid displayBatteryPercentage(SceVoid)
{
	int percent = scePowerGetBatteryLifePercent();
	drawSetColour(WHITE, getColour());
		
	if (percent == 100)
		drawStringf(880, 0, "%d %%", percent);
	else
		drawStringf(896, 0, "%d %%", percent);
}

SceVoid displayBatteryLifetime(SceVoid)
{
	int lifetime = scePowerGetBatteryLifeTime();
	drawSetColour(WHITE, getColour());
		
	if (R_SUCCEEDED(lifetime))
		drawStringfCenter(0, "%02ih %02im", lifetime / 60, lifetime - (lifetime / 60 * 60));
}

SceVoid displayBatteryTemp(SceVoid)
{
	int temp = scePowerGetBatteryTemp();
	drawSetColour(WHITE, getColour());
		
	if (temp > 0)
		drawStringf(0, 0, "Temp: %02i C", temp / 100);
}