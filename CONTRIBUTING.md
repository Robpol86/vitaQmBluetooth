# Contributing

TODO

## Bug Reports

TODO

## Pull Requests

TODO

### Setup Development Environment

TODO

TODO support vita3k

Install vitacompanion on a PS Vita:

```bash
# TODO replace with make commands.
# https://github.com/Robpol86/vitacompanion/tree/sdk-fix
#   rm -rf build && cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -B build . && cmake --build build
wget -P build-SCREENSHOT --mirror ftp://192.168.9.109:1337/ux0:/picture/SCREENSHOT/
curl -T build-debug/vitaQmBluetooth.suprx ftp://192.168.9.109:1337/ur0:/QuickMenuReborn/
echo reboot |nc -v 192.168.9.109 1338
```

Install Cat-A-Log on a PS Vita:

```bash
# https://github.com/isage/catlog
#   rm -rf build && cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -B build . && cmake --build build
#   for f in build/*_module/catlog.s?prx; do curl -T "$f" ftp://192.168.9.109:1337/ur0:/tai/; done
# Edit ur0:tai/config.txt
```

## Releases

TODO
