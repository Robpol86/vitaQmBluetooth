/*
This file is part of vitaQmBluetooth.
Copyright © 2026 Robpol86

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * Main entrypoint of the plugin.
 */

#include <psp2/io/stat.h>
#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <quickmenureborn/qm_reborn.h>
#include <stdbool.h>
#include <stdlib.h>

// Reference ids for each widget. Must be unique within the quick menu
// across all loaded QMR plugins, so they are namespaced with a plugin-
// specific prefix rather than using the upstream sample's bare names.
#define REF_PREFIX "vita_qm_bt_sample_"
#define BUTTON_REF_ID REF_PREFIX "button"
#define CHECKBOX_REF_ID REF_PREFIX "checkbox"
#define PLANE_ID REF_PREFIX "plane"
#define TEXT_ID REF_PREFIX "text"
#define CHECKBOX_TEXT_ID REF_PREFIX "checkbox_text"
#define SEPARATOR_ID REF_PREFIX "separator"
#define TEX_PLANE_ID REF_PREFIX "plane_for_tex"
#define TEXTURE_REF_ID REF_PREFIX "texture"

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

/**
 * Main entrypoint. Called when the module is started.
 *
 * @param args The size of the arguments passed to the module.
 * @param argp A pointer to the arguments passed to the module.
 * @return SCE_KERNEL_START_SUCCESS on success, or an error code on failure.
 */
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

    QuickMenuRebornRegisterWidget(TEXT_ID, NULL, text);
    QuickMenuRebornSetWidgetSize(TEXT_ID, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetColor(TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetPosition(TEXT_ID, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(TEXT_ID, "Tapper");

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

    QuickMenuRebornRegisterWidget(CHECKBOX_TEXT_ID, PLANE_ID, text);
    QuickMenuRebornSetWidgetColor(CHECKBOX_TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetSize(CHECKBOX_TEXT_ID, 500, 75, 0, 0);
    QuickMenuRebornSetWidgetPosition(CHECKBOX_TEXT_ID, -255, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(CHECKBOX_TEXT_ID, "Reset On Exit");

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

/**
 * Unloading entrypoint. Called when the module is stopped.
 *
 * @param args The size of the arguments passed to the module.
 * @param argp A pointer to the arguments passed to the module.
 * @return SCE_KERNEL_STOP_SUCCESS on success, or an error code on failure.
 */
int module_stop(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    QuickMenuRebornUnregisterWidget(BUTTON_REF_ID);
    QuickMenuRebornUnregisterWidget(CHECKBOX_REF_ID);
    QuickMenuRebornUnregisterWidget(TEXT_ID);
    QuickMenuRebornUnregisterWidget(CHECKBOX_TEXT_ID);
    QuickMenuRebornUnregisterWidget(PLANE_ID);
    QuickMenuRebornUnregisterWidget(TEX_PLANE_ID);
    QuickMenuRebornUnregisterTexture(TEXTURE_REF_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID);

    return SCE_KERNEL_STOP_SUCCESS;
}
