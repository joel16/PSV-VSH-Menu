#ifndef PSV_VSH_MENUS_H
#define PSV_VSH_MENUS_H

#define VSH_MAIN_MENU      1
#define VSH_BATTERY_MENU   2
#define VSH_PROGRAM_MENU   3

extern SceInt profile_max_battery[];
extern SceInt profile_default[];
extern SceInt profile_max_performance[];
extern SceInt profile_game[];
extern SceInt *profiles[4];

extern SceInt showVSH, selection;

SceUInt32 pressed_buttons, old_buttons;
SceUInt32 SCE_CTRL_ENTER, SCE_CTRL_CANCEL;

SceVoid Menu_Display(SceBool vertical_blank);
SceInt Menu_HandleControls(SceUInt32 pad);

#endif