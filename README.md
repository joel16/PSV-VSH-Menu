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

1) Put ```vsh.suprx``` and ```vsh.skprx``` in "tai" folder in the root of your Vita memory card.
2) Edit config.txt in that directory and add the lines below:

```text
*KERNEL
ux0:tai/vsh.skprx
```
3) Save config.txt.
4) Go to settings->Henkaku Settings->Reload tai cache and then reboot device.

Then use **L + R + START** to open the VSH menu on a game.

Credits
--------------------------------------------------------------------------------

- Rinnegatamante for kuio.
- frangarcj for oclockvita

