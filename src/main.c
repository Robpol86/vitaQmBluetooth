// vitaQmBluetooth — minimal QMR sample port (AI port of VDSuite sample).
// Direct port of the upstream QuickMenuReborn "Tapper" sample plugin from
// VDSuite headers to VitaSDK headers. No feature changes; this is a
// scaffold to prove the build pipeline end-to-end. Bluetooth toggle
// logic will replace this body in a follow-up commit.

#include <psp2/io/stat.h>
#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <quickmenureborn/qm_reborn.h>
#include <stdbool.h>
#include <stdlib.h>

// Widget IDs (prefixed because they must be unique across all plugins)
#define PROJECT_NAME "vitaQmBluetooth"
#define SEPARATOR_ID PROJECT_NAME "Separator"
#define SECTION_TEXT_ID PROJECT_NAME "SectionText"

int module_start(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    QuickMenuRebornSeparator(SEPARATOR_ID, SCE_SEPARATOR_HEIGHT);

    QuickMenuRebornRegisterWidget(SECTION_TEXT_ID, NULL, text);
    QuickMenuRebornSetWidgetSize(SECTION_TEXT_ID, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetColor(SECTION_TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetPosition(SECTION_TEXT_ID, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(SECTION_TEXT_ID, "Bluetooth Devices");

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    QuickMenuRebornUnregisterWidget(SECTION_TEXT_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID);

    return SCE_KERNEL_STOP_SUCCESS;
}
