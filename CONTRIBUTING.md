# Contributing

This document will guide you through setting up your local environment to start developing for this project, and also steps
needed to get a PS Vita ready for development.

## Bug Reports

TODO

## Pull Requests

TODO

## Setup Local Development Environment

TODO

### macOS

TODO

### Linux

TODO

### Windows

TODO Use WSL

## Setup Development PS Vita

This project's [Makefile](Makefile) is designed to quickly deploy new development builds to a hardware PS Vita running
**vitacompanion** and **Cat-A-Log**.

### vitacompanion

[vitacompanion](https://github.com/devnoname120/vitacompanion) provides an always-on FTP server that this project uses to:

- Upload new builds to the PS Vita
- Download logs and screenshots from the PS Vita
- Reboot the PS Vita

To install vitacompanion on the PS Vita:

1. Download **vitacompanion.suprx** from: https://github.com/devnoname120/vitacompanion/releases/latest
1. Upload the file to the PS Vita into the directory: **ur0:tai/**
1. Add this to **ur0:tai/config.txt**:
    ```text
    *main
    ur0:tai/vitacompanion.suprx
    ```
1. Reboot the PS Vita
1. Get the PS Vita's IP address: Settings > Network > Internet Connection Status

#### Verify

Take a screenshot (PS Button + Start) and then run (replace 192.168.0.100 with your PS Vita's IP address):

```bash
make PSVITA_IP=192.168.0.100 fetch-screenshots
ls fetch-screenshots
```

For more commands, run:

```bash
make help
```

> [!NOTE]
> If you get **Access failed: 550 Invalid directory** I've opened a bug report for it here:
> https://github.com/devnoname120/vitacompanion/issues/20
>
> The current solution is to build 7a9a1a44c648bf995bfbd11c00fd3616dcc6bd7c yourself.

### Cat-A-Log

[Cat-A-Log](https://github.com/isage/catlog) is used to read logs live from the PS Vita in your development machine's
terminal.

To install Cat-A-Log on the PS Vita:

1. Download **catlog.suprx** and **catlog.skprx** from: https://github.com/isage/catlog/releases/latest
1. Upload both files to the PS Vita into the directory: **ur0:tai/**
1. Add this to **ur0:tai/config.txt**:
    ```text
    *KERNEL
    ur0:tai/catlog.skprx
    *NPXS10015
    ur0:tai/catlog.suprx
    ```
1. Reboot the PS Vita
1. Get your computer's IP address, then input it in: Settings > Network > Cat Log > Log host
    1. Set **Log port** to: 10224
    1. Then return to main Settings menu
1. In your local terminal run `make recv-logs`, you should instantly see a lot of logs appear
    1. If you don't, try rebooting. If you still don't, make sure you've set the right IP address and that you can ping the
       PS Vita from your computer.

## Releases

TODO
