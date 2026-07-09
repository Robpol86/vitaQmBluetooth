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

TODO

Copy from local to dev vita:

```bash
cp ./redist/quickmenureborn/* /Volumes/BLUEVITA/downloads/
cp ./build-debug/module_*/vitaQmBluetooth.s?prx /Volumes/BLUEVITA/downloads/
```

Unmount, then in VitaShell:

1. Move to `ur0:tai/`
    1. `ux0:downloads/vitaQmBluetooth.skprx`
    1. `ux0:downloads/QuickMenuReborn.suprx`
1. Move to `ur0:QuickMenuReborn/` (mkdir)
    1. `ux0:downloads/qmr_plugin.rco`
    1. `ux0:downloads/vitaQmBluetooth.suprx`
1. Append to `ur0:tai/config.txt`
    1.  ```
        *KERNEL
        ur0:tai/vitaQmBluetooth.skprx
        *main
        ur0:tai/QuickMenuReborn.suprx
        ```
1. Reboot

## Attributions

This plugin uses Ibrahim778's [QuickMenuReborn](https://github.com/Ibrahim778/QuickMenuReborn).
