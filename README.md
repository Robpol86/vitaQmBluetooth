# vitaQmBluetooth

Reconnect bluetooth devices from the [quick menu](https://manuals.playstation.net/document/gb/psvita/basic/menu.html) without
needing to open the Settings app.

I developed this plugin because I was tired of having to close RetroArch to reconnect my AirPods Pro 1 headphones. This
plugin got me to finally ditch my wired headphones for my PS Vita when I'm on a flight.

![Screenshot](example.png?raw=true "Screenshot")

> [!TIP]
> In case you didn't know, you can open the quick menu by holding down the **PS Button** on your PS Vita or PSTV's DS3. With
> the plugin installed you can scroll to the bottom to see the new "Bluetooth Devices" section.

## Install

Download the files for the latest version from the releases page: https://github.com/Robpol86/vitaQmBluetooth/releases/latest

Upload the four files to your PS Vita to the following locations:

| File                  | Vita Folder            |
| --------------------- | ---------------------- |
| vitaQmBluetooth.skprx | `ur0:tai/`             |
| QuickMenuReborn.suprx | `ur0:tai/`             |
| vitaQmBluetooth.suprx | `ur0:QuickMenuReborn/` |
| qmr_plugin.rco        | `ur0:QuickMenuReborn/` |

Then add this to your `ur0:tai/config.txt` file:

```
*KERNEL
ur0:tai/vitaQmBluetooth.skprx
*main
ur0:tai/QuickMenuReborn.suprx
```

Then reboot the PS Vita.

## Attributions

This plugin uses Ibrahim778's [QuickMenuReborn](https://github.com/Ibrahim778/QuickMenuReborn).
