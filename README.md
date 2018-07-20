# PSV-VSH-Menu ![Github latest downloads](https://img.shields.io/github/downloads/joel16/PSV-VSH-Menu/total.svg)
A VSH menu taiHEN plugin for the PS VITA games and homebrews. Currently does not work in SceShell.


Features
--------------------------------------------------------------------------------
- Control CPU/GPU clock states separately for each title and save them.
- Restore configured CPU/GPU clock states, once plugin has been started.
- Battery percent, hours remaining as well as battery temperature display can be toggled.
- VSH Menu colours for minimal customization.
- Quick launch with 5 slots. (Once the launcher.cfg file is generated in ur0:/data/vsh/launcher.cfg, feel free to change the title and title IDS to your preference. Keep in mind you can't use spaces for the title's name and you can't go above 5 (0-4) slots in the meantime.)
- Quick Reboot/Shutdown/Suspend/Restart device in game.


Installation
--------------------------------------------------------------------------------

Put ```vsh.suprx``` and ```kuio.skprx``` in "tai" folder in the root of your Vita **(ur0:/tai/)**.

Edit config.txt in that directory to load plugin for title of your choice by adding new lines like below:

```text
*KERNEL
ur0:tai/kuio.skprx

*!PSPEMUCFW
*ALL
ur0:tai/vsh.suprx
```

Then use **L + R + START** or **L + R + SELECT** to open the VSH menu.


Credits
--------------------------------------------------------------------------------

- Rinnegatamante for kuio.
- frangarcj for oclockvita
