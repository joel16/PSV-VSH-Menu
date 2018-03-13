# PSV-VSH-Menu ![Github latest downloads](https://img.shields.io/github/downloads/joel16/PSV-VSH-Menu/total.svg)
A VSH menu taiHEN plugin for the PS VITA. Currently does not work in SceShell.

Features
--------------------------------------------------------------------------------
- Control CPU/GPU clock states separately for each title.
- Restore configured CPU/GPU clock states, once plugin has been started.
- Battery percent, hours remaining as well as battery temperature display can be toggled.
- VSH Menu colours for minimal customization.
- Quick launch with 5 slots. (Once the launcher.cfg file is generated in ur0:/data/vsh/launcher.cfg, feel free to change the title and title IDS to your preference. Keep in mind you can't use spaces for the title's name and you can't go above 5 (0-4) slots in the meantime.)
- Quick Reboot/Shutdown/Suspend device in game.

Installation
--------------------------------------------------------------------------------

Put "vsh.suprx" in "tai" folder in the root of your Vita memory card.

Edit config.txt in that directory to load plugin for title of your choice by adding new lines like below:

For version 2.0 and above you'll need a slightly modified (lighter) version of kuio.skprx (included in the releases tab).

```text
*KERNEL
ux0:tai/kuio.skprx

*ALL
ux0:tai/vsh.suprx
```

Then use **L + R + START** to open the VSH menu.

Credits
--------------------------------------------------------------------------------

- Rinnegatamante for kuio.
- frangarcj for oclockvita

