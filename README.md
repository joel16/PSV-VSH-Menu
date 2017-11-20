# PSV-VSH-Menu ![Github latest downloads](https://img.shields.io/github/downloads/joel16/PSV-VSH-Menu/total.svg)
A VSH menu taiHEN plugin for the PS VITA.

Features
--------------------------------------------------------------------------------
- Control CPU/GPU clock states separately.
- Restore configured CPU/GPU clock states, once plugin has been started.
- Battery percent display toggle.
- VSH Menu colours for minimal customization.
- Quick launch Settings or VitaShell.
- Quick Reboot/Shutdown/Suspend device in game.

Recovery features: (You need PSV VSH Recovery for this)
- Reload tai config.
- Back up PSN activation files.
- Rebuild/Update database.
- Swap enter button (X / O)


Installation
--------------------------------------------------------------------------------

Put "vsh.suprx" in "tai" folder in the root of your Vita memory card.

Edit config.txt in that directory to load plugin for title of your choice by adding new lines like below:

```text
# titleid for your game (this one is for Root//Letter for example)
*PCSB01019
ux0:tai/vsh.suprx
```

Then use **L + R + START** to open the VSH menu.
