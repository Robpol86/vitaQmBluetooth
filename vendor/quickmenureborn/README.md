# QuickMenuReborn (vendored)

This directory contains a snapshot of QuickMenuReborn's developer package,
vendored into the repository so the plugin can build without requiring users
(or CI) to install QMR into `$VITASDK` first, and so the build remains
reproducible even if the upstream repository becomes unavailable.

## Upstream

- Source: https://github.com/Ibrahim778/QuickMenuReborn
- Release: **V2.5**
- Commit: `785f6d41b78e1fcade91fb309ebe5b32a3af3b2c`
- Download: https://github.com/Ibrahim778/QuickMenuReborn/releases/download/V2.5/dev_pkg.zip
- License: MIT (see `LICENSE`)

## Contents

| Path | Source in `dev_pkg.zip` |
| --- | --- |
| `include/quickmenureborn/qm_reborn.h` | `quickmenureborn/qm_reborn.h` |
| `include/quickmenureborn/c_types.h` | `quickmenureborn/c_types.h` |
| `lib/libQuickMenuReborn_stub.a` | `vitasdk_stubs/libQuickMenuReborn_stub.a` |
| `lib/libQuickMenuReborn_stub_weak.a` | `vitasdk_stubs/libQuickMenuReborn_stub_weak.a` |

The VDSuite stubs under `dev_pkg.zip/stubs/` are intentionally not vendored —
this project targets VitaSDK only.

## Runtime dependency (NOT vendored)

End users must also install the QMR **plugin** itself (`QuickMenuReborn.suprx`
plus `qmr_plugin.rco`) from the same release. That's a runtime concern and
lives on the user's Vita, not in this repository. See the plugin README for
user install instructions.

## Updating

1. Download the latest `dev_pkg.zip` from the release URL above.
2. Extract it somewhere temporary.
3. Replace the files in `include/quickmenureborn/` and `lib/` with the
   corresponding files from the archive (see the Contents table).
4. Update the **Release** and **Commit** lines above. The commit hash can be
   found on the GitHub release page.
5. Run `nm lib/libQuickMenuReborn_stub_weak.a | grep ' T '` and verify that
   any new API symbols referenced by `src/main.c` are present, and that any
   previously-used symbols have not been removed.
6. Rebuild and confirm the plugin still loads on a Vita running the
   corresponding QMR plugin version.

## License compliance

QMR is distributed under the MIT license. The `LICENSE` file in this directory
is an unmodified copy of the upstream license, preserving the copyright notice
as required. No source modifications are made to the vendored files.
