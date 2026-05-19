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

/******************************************************************************
 * @file
 * @brief Interface for the Quick Menu on the PS Vita.
 ******************************************************************************/

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <quickmenureborn/qm_reborn.h>

#include "log.h"
#include "vqmbt.h"

// Widget IDs (prefixed because they must be unique across all plugins).
#define ID_SEPARATOR MODULE_NAME "Separator"
#define ID_PLANE_ROOT MODULE_NAME "PlaneRoot"
#define ID_SECTION_TEXT MODULE_NAME "SectionText"
static const char* const ID_BUTTONS[VQMBT_MAX_DEVICES] = {
    MODULE_NAME "Button0", MODULE_NAME "Button1", MODULE_NAME "Button2", MODULE_NAME "Button3",
    MODULE_NAME "Button4", MODULE_NAME "Button5", MODULE_NAME "Button6", MODULE_NAME "Button7",
};
_Static_assert(sizeof(ID_BUTTONS) / sizeof(ID_BUTTONS[0]) == VQMBT_MAX_DEVICES,
               "ID_BUTTONS size must match VQMBT_MAX_DEVICES");

/**
 * Called when the user taps on the button. Emits a log message.
 * TODO:
 * - on_press: if button is No Device do nothing; else connect/disconnect; relabel button "Connecting <name>..."
 * - when user taps a button disable all buttons and wait for callback.
 */
BUTTON_HANDLER(on_press) {
    (void)id;
    (void)hash;
    (void)eventId;
    (void)userDat;

    // name(const char *id, SceInt32 hash, SceInt32 eventId, void *userDat)
    LOG_DEBUG(0, "Button pressed TODO log all params");
}

/**
 * TODO
 *
 * TODO:
 * - callback: relabel button with new state. Surface error in button as close/reopen resets labels
 * - update screenshot
 */
void add_buttons(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornRegisterWidget(id, ID_PLANE_ROOT, button);
        QuickMenuRebornSetWidgetSize(id, 600, 75, 0, 0);
        QuickMenuRebornSetWidgetPosition(id, 20, 243 - (idx * 80), 0, 0);
        QuickMenuRebornSetWidgetColor(id, 1, 1, 1, 1);
        char label[32];
        sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        QuickMenuRebornSetWidgetLabel(id, label);
        QuickMenuRebornRegisterEventHanlder(id, QMR_BUTTON_RELEASE_ID, on_press, NULL);
    }
}

/**
 * Called when the quick menu is opened by the user.
 * TODO:
 * - on_load: query kernel, update button text with Con/Disc prefix based on state
 * - Depending on state label it "Connect <device>" or "Disconnect <device>".
 */
ONLOAD_HANDLER(on_load) {
    (void)id;

    LOG_DEBUG(0, "Quick menu opened.");

    // Query kernel.
    // TODO query, update label, pass mac0/1 or idx (static?) to on_press via userDat?
}

/**
 * Called when the quick menu is closed by the user.
 */
void on_unload(const char* id) {
    (void)id;

    LOG_DEBUG(0, "Quick menu closed.");

    // Reset button labels.
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        char label[32];
        sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        QuickMenuRebornSetWidgetLabel(id, label);
    }
}

/**
 * Loads the plugin's quick menu items.
 *
 * TODO:
 * - Add function to calculate position from top left instead of center.
 * - Pixel perfect alignment.
 * - If kernel plugin isn't loaded notify user.
 */
void quickmenu_start(void) {
    // Add horizontal line separator.
    QuickMenuRebornSeparator(ID_SEPARATOR, SCE_SEPARATOR_HEIGHT);

    // Add the root plane that holds all other items.
    QuickMenuRebornRegisterWidget(ID_PLANE_ROOT, NULL, plane);
    QuickMenuRebornSetWidgetSize(ID_PLANE_ROOT, SCE_PLANE_WIDTH, 700, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_PLANE_ROOT, 1, 1, 1, 0);

    // Add section heading text.
    QuickMenuRebornRegisterWidget(ID_SECTION_TEXT, ID_PLANE_ROOT, text);
    QuickMenuRebornSetWidgetSize(ID_SECTION_TEXT, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_SECTION_TEXT, -206, 312, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_SECTION_TEXT, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_SECTION_TEXT, "Bluetooth Devices");

    // TODO.
    add_buttons();

    // Register handlers.
    QuickMenuRebornAssignOnLoadHandler(on_load, ID_PLANE_ROOT);
    QuickMenuRebornAssignOnDeleteHandler(on_unload, ID_PLANE_ROOT);
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornUnregisterWidget(id);
    }
    QuickMenuRebornUnregisterWidget(ID_SECTION_TEXT);
    QuickMenuRebornUnregisterWidget(ID_PLANE_ROOT);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);
}
