#ifndef PSV_VSH_CONFIG_H
#define PSV_VSH_CONFIG_H

#include <vitasdk.h>

typedef struct
{
	SceInt colour;
	SceBool battery_percent;
	SceBool battery_lifetime;
	SceBool battery_temp;
	SceBool battery_keep_display;
	SceBool fps_display;
	SceBool fps_keep_display;
	SceBool clock_display;
	SceBool clock_keep_display;
} Menu_Config_t;

typedef struct
{
    SceUInt32 title_bg_col;
    SceUInt32 bg_col;
    SceUInt32 cursor_col;
    SceUInt32 title_text_col;
    SceUInt32 text_col;
} Custom_Colour_t;

typedef struct
{
	SceInt c_clock;
	SceInt g_clock;
	SceInt refresh_interval;
} Clock_Config_t;

Menu_Config_t Menu_Config;
Clock_Config_t Clock_Config;
Custom_Colour_t Custom_Colour;

char titleID[16];
char app_title[5][50], app_titleID[5][50];

SceInt Config_SaveMenuConfig(Menu_Config_t Menu_Config);
SceInt Config_SaveClockConfig(Clock_Config_t Clock_Config);
SceInt Config_LoadConfig(SceVoid);
SceInt Config_GetVSHColour(SceVoid);
SceUInt64 Config_GetInterval(SceVoid);

#endif