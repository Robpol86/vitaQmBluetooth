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

// Reference ids for each widget. Must be unique within the quick menu
// across all loaded QMR plugins, so they are namespaced with a plugin-
// specific prefix rather than using the upstream sample's bare names.
#define PROJECT_NAME "vitaQmBluetooth"
#define SECTION_TEXT_ID PROJECT_NAME "SectionText"
#define NO_DEVICES_TEXT_ID PROJECT_NAME "NoDevicesText"
#define BUTTON_REF_ID PROJECT_NAME "button"
#define CHECKBOX_REF_ID PROJECT_NAME "checkbox"
#define PLANE_ID PROJECT_NAME "plane"
#define SEPARATOR_ID PROJECT_NAME "separator"
#define TEX_PLANE_ID PROJECT_NAME "plane_for_tex"
#define TEXTURE_REF_ID PROJECT_NAME "texture"

// Test texture path matches the upstream sample; users without VitaShell
// installed will simply not see the textured plane.
#define TEST_TEXTURE_PATH "ux0:app/VITASHELL/sce_sys/icon0.png"

static int count = 0;
static bool reset_on_exit = false;

BUTTON_HANDLER(on_press) {
    (void)id;
    (void)hash;
    (void)eventId;
    (void)userDat;

    count++;

    char new_text[0x100];  // 0x400 is the max label size per QMR
    sceClibSnprintf(new_text, sizeof(new_text), "You Pressed Me %d Times", count);

    float x = 400;
    if (count >= 100) x += 50;
    if (count >= 1000) x += 50;

    QuickMenuRebornSetWidgetSize(BUTTON_REF_ID, x, 75, 0, 0);
    QuickMenuRebornSetWidgetLabel(BUTTON_REF_ID, new_text);
}

ONLOAD_HANDLER(on_button_load) {
    (void)id;

    if (reset_on_exit) {
        count = 0;
        QuickMenuRebornSetWidgetSize(BUTTON_REF_ID, 200, 75, 0, 0);
        QuickMenuRebornSetWidgetLabel(BUTTON_REF_ID, "Press Me!");
    }
}

BUTTON_HANDLER(on_toggle_checkbox) {
    (void)id;
    (void)hash;
    (void)eventId;
    (void)userDat;

    reset_on_exit = (QuickMenuRebornGetCheckboxValue(CHECKBOX_REF_ID) != 0);
}

int module_start(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    QuickMenuRebornSeparator(SEPARATOR_ID, SCE_SEPARATOR_HEIGHT);

    // Restore checkbox's saved state. If no saved value exists, default to
    // false rather than treating the error code as a truthy int.
    int ret = QuickMenuRebornGetCheckboxValue(CHECKBOX_REF_ID);
    if (ret == QMR_CONFIG_MGR_ERROR_NOT_EXIST) {
        reset_on_exit = false;
    } else {
        reset_on_exit = (ret != 0);
    }

    QuickMenuRebornRegisterWidget(SECTION_TEXT_ID, NULL, text);
    QuickMenuRebornSetWidgetSize(SECTION_TEXT_ID, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetColor(SECTION_TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetPosition(SECTION_TEXT_ID, -250, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(SECTION_TEXT_ID, "Bluetooth Devices");

    QuickMenuRebornRegisterWidget(PLANE_ID, NULL, plane);
    QuickMenuRebornSetWidgetSize(PLANE_ID, SCE_PLANE_WIDTH, 100, 0, 0);
    QuickMenuRebornSetWidgetColor(PLANE_ID, 1, 1, 1, 0);

    QuickMenuRebornRegisterWidget(CHECKBOX_REF_ID, PLANE_ID, check_box);
    QuickMenuRebornSetWidgetSize(CHECKBOX_REF_ID, 48, 48, 0, 0);
    QuickMenuRebornSetWidgetColor(CHECKBOX_REF_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetPosition(CHECKBOX_REF_ID, 350, 0, 0, 0);
    QuickMenuRebornAssignDefaultCheckBoxRecall(CHECKBOX_REF_ID);
    QuickMenuRebornAssignDefaultCheckBoxSave(CHECKBOX_REF_ID);
    QuickMenuRebornRegisterEventHanlder(CHECKBOX_REF_ID, QMR_BUTTON_RELEASE_ID, on_toggle_checkbox, NULL);

    QuickMenuRebornRegisterWidget(NO_DEVICES_TEXT_ID, PLANE_ID, text);
    QuickMenuRebornSetWidgetColor(NO_DEVICES_TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetSize(NO_DEVICES_TEXT_ID, 500, 75, 0, 0);
    QuickMenuRebornSetWidgetPosition(NO_DEVICES_TEXT_ID, -200, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(NO_DEVICES_TEXT_ID, "No Bluetooth devices paired");

    QuickMenuRebornRegisterWidget(BUTTON_REF_ID, NULL, button);
    QuickMenuRebornSetWidgetSize(BUTTON_REF_ID, 200, 75, 0, 0);
    QuickMenuRebornSetWidgetColor(BUTTON_REF_ID, 1, 1, 1, 1);
    QuickMenuRebornRegisterEventHanlder(BUTTON_REF_ID, QMR_BUTTON_RELEASE_ID, on_press, NULL);
    QuickMenuRebornSetWidgetLabel(BUTTON_REF_ID, "Press Me!");
    QuickMenuRebornAssignOnLoadHandler(on_button_load, BUTTON_REF_ID);

    SceIoStat s;
    if (sceIoGetstat(TEST_TEXTURE_PATH, &s) >= 0) {
        QuickMenuRebornRegisterWidget(TEX_PLANE_ID, NULL, plane);
        QuickMenuRebornRegisterTexture(TEXTURE_REF_ID, TEST_TEXTURE_PATH);
        QuickMenuRebornSetWidgetSize(TEX_PLANE_ID, 128, 128, 0, 0);
        QuickMenuRebornSetWidgetColor(TEX_PLANE_ID, 1, 1, 1, 1);
        QuickMenuRebornSetWidgetTexture(TEX_PLANE_ID, TEXTURE_REF_ID);
    }

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    QuickMenuRebornUnregisterWidget(BUTTON_REF_ID);
    QuickMenuRebornUnregisterWidget(CHECKBOX_REF_ID);
    QuickMenuRebornUnregisterWidget(SECTION_TEXT_ID);
    QuickMenuRebornUnregisterWidget(NO_DEVICES_TEXT_ID);
    QuickMenuRebornUnregisterWidget(PLANE_ID);
    QuickMenuRebornUnregisterWidget(TEX_PLANE_ID);
    QuickMenuRebornUnregisterTexture(TEXTURE_REF_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID);

    return SCE_KERNEL_STOP_SUCCESS;
}
