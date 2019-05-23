# PSV-VSH-Menu ![Github latest downloads](https://img.shields.io/github/downloads/joel16/PSV-VSH-Menu/total.svg)
A VSH menu taiHEN plugin for the PS VITA games and homebrews. Currently does not work in SceShell.

# This plugin is no longer maintained. Please use something else or don't use it. Any issues reported will be ignored.

Features
--------------------------------------------------------------------------------
- Control CPU/GPU clock states separately for each title and save them.
- Restore configured CPU/GPU clock states, once plugin has been started.
- Battery percent, hours remaining as well as battery temperature display can be toggled.
- VSH Menu colours for minimal customization as well as an option to set your own colours.
- Quick launch with 5 slots. (Once the launcher.cfg file is generated in ur0:/data/vsh/launcher.cfg, feel free to change the title and title IDS to your preference. Keep in mind you can't use spaces for the title's name and you can't go above 5 (0-4) slots in the meantime.)
- Quick Reboot/Shutdown/Suspend/Restart device in game.
- FPS display.


Installation
--------------------------------------------------------------------------------

Put ```vsh.suprx``` and ```kuio.skprx``` in "tai" folder in the root of your Vita *(ur0:/tai/)*.
**If you already have kuio installed from another plugin then you don't need to use the one bundlded in VSH's release.**

Edit config.txt in that directory to load plugin for title of your choice by adding new lines like below:

```text
*KERNEL
ur0:tai/kuio.skprx

*!PSPEMUCFW
*ALL
ur0:tai/vsh.suprx
```

Go to settings -> Henkaku settings -> Reload tai config -> Reboot device.
Then use **L + R + START** or **L + R + SELECT** to open the VSH menu.


Custom colours
--------------------------------------------------------------------------------

Uses 8 digit hex codes -> AARRGGBB.

Open ```ur0:/data/vsh/custom_colour_config.cfg``` using your computer, or VITAShell and edit the following to your liking.

```
TITLE_BG_COL = Title background colour
BG_COL = Background colour
CURSOR_COL = Cursor/selector colour
TITLE_TEXT_COL = Title font colour
TEXT_COL = Font colour
```


Credits
--------------------------------------------------------------------------------

- Rinnegatamante for kuio and FPS code.
- frangarcj for oclockvita
