#ifndef PSV_VSH_MENUS_H
#define PSV_VSH_MENUS_H

extern SceInt profile_max_battery[];
extern SceInt profile_default[];
extern SceInt profile_max_performance[];
extern SceInt profile_game[];
extern SceInt *profiles[4];

extern SceInt showVSH;

SceUInt32 SCE_CTRL_ENTER, SCE_CTRL_CANCEL;

SceInt Menu_HandleControls(SceVoid);

#endif