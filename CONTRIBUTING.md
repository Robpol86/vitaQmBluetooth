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
[vitacompanion](https://github.com/devnoname120/vitacompanion) and [Cat-A-Log](https://github.com/isage/catlog).

### vitacompanion

[vitacompanion](https://github.com/devnoname120/vitacompanion) provides an always-on FTP server that this project uses to:

- Upload new builds to the PS Vita
- Download logs and screenshots from the PS Vita
- Reboot the PS Vita

To install vitacompanion on the PS Vita:

1. Download **vitacompanion.suprx** from: https://github.com/devnoname120/vitacompanion/releases/latest
1. Install the suprx by:
    1. Uploading it to the PS Vita in the directory: **ur0:tai/**
    1. Add this line to **ur0:tai/config.txt**:
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

Install Cat-A-Log on a PS Vita:

```bash
# https://github.com/isage/catlog
#   rm -rf build && cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -B build . && cmake --build build
#   for f in build/*_module/catlog.s?prx; do curl -T "$f" ftp://192.168.9.109:1337/ur0:/tai/; done
# Edit ur0:tai/config.txt
# Add firewall rule to router.
# Local: nc -kl 10224
# Reboot vita to get a lot of kernel log statements.
```

## Releases

TODO
