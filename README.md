# vitaQmBluetooth

Reconnect bluetooth devices from the quick menu without needing to open the Settings app

> ⚠️ ⚠️ ⚠️ [NOTICE] ⚠️ ⚠️ ⚠️
> The project is currently under development.

## Install

Copy from local to dev vita:

```bash
cp ./redist/quickmenureborn/* /Volumes/BLUEVITA/downloads/
cp ./build-debug/vitaQmBluetooth.suprx /Volumes/BLUEVITA/downloads/
```

Unmount, then in VitaShell:

1. Copy to `ur0:tai/`
    1. `ux0:downloads/QuickMenuReborn.suprx`
1. Copy to `ur0:QuickMenuReborn/` (mkdir)
    1. `ux0:downloads/qmr_plugin.rco`
    1. `ux0:downloads/vitaQmBluetooth.suprx`
1. Append to `ur0:tai/config.txt`
    1.  ```
        *main
        ur0:tai/QuickMenuReborn.suprx
        ```
1. Reboot

## Usage

Hold down the PS button and scroll to the bottom of the quick menu.

![Screenshot](example.png?raw=true "Screenshot")
