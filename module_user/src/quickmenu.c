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

/**
 * TODO:
 * - Stick with QMR.
 *      - Always show 8 buttons
 *      - Write disable_button() enable_button() to grey and no-op callbacks
 *      - When bt is disabled disable all buttons and relabel each with "bt is disabled"
 * - Implement button state struct array
 * - PR merge
 * - Bring in QMR APIs into paf.cpp here (keep project C, paf.cpp will have C++ stubs) one by one
 *      - Eventually add new APIs such as Show/Hide and Enable/Disable
 *      - Hide/show the root plane, confirm buttons are unselectable whilst hidden, and no scrollbars
 *      - Enable/disable first button. Should be selectable as-per big BT button at the top, but tap/X/O no-ops
 */

#include "quickmenu.h"

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr/thread.h>
#include <quickmenureborn/c_types.h>
#include <quickmenureborn/qm_reborn.h>

#include "kmod_event.h"
#include "log.h"
#include "vqmbt.h"

#define BUTTON_LABEL_MAX (VQMBT_DEVICE_NAME_MAX + 16)

// Widget IDs (prefixed because they must be unique across all plugins).
#define ID_SEPARATOR MODULE_NAME "Separator"
#define ID_SECTION_TITLE MODULE_NAME "SectionTitle"
#define ID_PLANE_BUTTONS MODULE_NAME "PlaneButtons"

// Buttons struct that maintains state of a button.
typedef enum QmButtonState : unsigned int {
    BTNSTATE_DISCONNECTED = 0,
    BTNSTATE_DISCONNECTING,
    BTNSTATE_CONNECTED,
    BTNSTATE_CONNECTING,
} QmButtonState;
typedef struct QmButton {
    char device_name[VQMBT_DEVICE_NAME_MAX];
    unsigned int mac0;
    unsigned int mac1;
    QmButtonState state;
    bool enabled;
    char qm_widget_id[sizeof(MODULE_NAME "Button00")];
} QmButton;
static QmButton qm_buttons[VQMBT_MAX_DEVICES];

/**
 * TODO
 */
static void refresh_buttons(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        QmButton* qm_button = &qm_buttons[idx];

        // Update label.
        char label[BUTTON_LABEL_MAX];
        if (qm_button->device_name[0] == '\0') {
            sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        } else {
            switch (qm_button->state) {
                case BTNSTATE_DISCONNECTED:
                    sceClibSnprintf(label, sizeof(label), "Connect %s", qm_button->device_name);
                    break;
                case BTNSTATE_DISCONNECTING:
                    sceClibSnprintf(label, sizeof(label), "Disconnecting %s", qm_button->device_name);
                    break;
                case BTNSTATE_CONNECTED:
                    sceClibSnprintf(label, sizeof(label), "Disconnect %s", qm_button->device_name);
                    break;
                case BTNSTATE_CONNECTING:
                    sceClibSnprintf(label, sizeof(label), "Connecting %s", qm_button->device_name);
                    break;
            }
        }
        QuickMenuRebornSetWidgetLabel(qm_button->qm_widget_id, label);
    }
}

/**
 * TODO
 */
static void reset(void) {
    // Flush kernel buffer.
    int ret = kvqmbt_read_event(NULL, 0);
    if (ret < 0) {
        LOG_ERROR("kvqmbt_read_event returned error 0x%08X", ret);
    }

    // Get all currently paired/registered bluetooth devices.
    VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES];
    int count = kvqmbt_get_paired_devices(devices, VQMBT_MAX_DEVICES);
    if (count < 0) {
        LOG_ERROR("kvqmbt_get_paired_devices returned error 0x%08X", count);
        return;
    }

    // Update buttons.
    // TODO.

    refresh_buttons();
}

/**
 * Connect or disconnect the device associated with the button.
 *
 * Called when the user taps on the button.
 *
 * TODO:
 * - Relabel button "Connecting <name>...".
 * - when user taps a button disable all buttons and wait for callback.
 */
static BUTTON_HANDLER(quickmenu_on_press) {
    (void)id;
    (void)hash;
    (void)eventId;
    int idx = (int)(intptr_t)userDat;

    LOG_DEBUG(0, "NOT IMPLEMENTED YET (idx=%d)", idx);
}

/**
 * Called when the quick menu is opened by the user.
 *
 * TODO:
 * - Performance? delay opening qm? async?
 * - Hide empty slots and resize plane to eliminate ghost scrolling.
 */
static ONLOAD_HANDLER(quickmenu_on_load) {
    (void)id;

    LOG_DEBUG(0, "Quick menu opened.");

    // Reset button states and fetch initial data.
    reset();  // TODO do this from a thread to not block quickmenu? Maybe use SceKernelLwMutexWork?

    // Start event thread for subsequent changes.
    kmod_event_start();
}

/**
 * Called when the quick menu is closed by the user.
 */
static void quickmenu_on_unload(const char* id) {
    (void)id;

    LOG_DEBUG(0, "Quick menu closed.");

    // Stop event thread.
    kmod_event_stop();
}

/**
 * Loads the plugin's quick menu items.
 *
 * TODO:
 * - Add function to calculate position from top left instead of center.
 * - Pixel perfect alignment.
 * - If kernel plugin isn't loaded notify user.
 * - If kernel plugin loaded AFTER user plugin what happens? probably fails. Handle this too.
 * TODO:
 * - quickmenu_state struct array
 * - quickmenu_start will start event thread, stop will shut down the thread.
 * - Two body planes: one for the buttons, another with just a text message.
 * TODO:
 * - callback: relabel button with new state. Surface error in button as close/reopen resets labels
 * - button_reset() button_disable() button_enable() functions
 * - long bt names ellipses
 */
void quickmenu_start(void) {
    // Add horizontal line separator.
    QuickMenuRebornSeparator(ID_SEPARATOR, SCE_SEPARATOR_HEIGHT);

    // Add section heading text.
    QuickMenuRebornRegisterWidget(ID_SECTION_TITLE, NULL, text);
    QuickMenuRebornSetWidgetSize(ID_SECTION_TITLE, 300, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_SECTION_TITLE, -206, 0, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_SECTION_TITLE, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_SECTION_TITLE, "Bluetooth Devices");

    // Add the button plane that holds all the buttons.
    QuickMenuRebornRegisterWidget(ID_PLANE_BUTTONS, NULL, plane);
    QuickMenuRebornSetWidgetSize(ID_PLANE_BUTTONS, SCE_PLANE_WIDTH, 650, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_PLANE_BUTTONS, 1, 1, 1, 0);

    // Add device buttons.
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        QmButton* qm_button = &qm_buttons[idx];

        // Set widget ID.
        sceClibSnprintf(qm_button->qm_widget_id, sizeof(qm_button->qm_widget_id), MODULE_NAME "Button%d", idx);

        // Create widget.
        QuickMenuRebornRegisterWidget(qm_button->qm_widget_id, ID_PLANE_BUTTONS, button);
        QuickMenuRebornSetWidgetSize(qm_button->qm_widget_id, 600, 75, 0, 0);
        QuickMenuRebornSetWidgetPosition(qm_button->qm_widget_id, 20, 280 - (idx * 80), 0, 0);
        QuickMenuRebornSetWidgetColor(qm_button->qm_widget_id, 1, 1, 1, 1);
        QuickMenuRebornRegisterEventHanlder(qm_button->qm_widget_id, QMR_BUTTON_RELEASE_ID, quickmenu_on_press,
                                            (void*)(intptr_t)idx);
    }

    // Register handlers.
    const char* last_widget = qm_buttons[VQMBT_MAX_DEVICES - 1].qm_widget_id;
    QuickMenuRebornAssignOnLoadHandler(quickmenu_on_load, last_widget);
    QuickMenuRebornAssignOnDeleteHandler(quickmenu_on_unload, last_widget);
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = qm_buttons[idx].qm_widget_id;
        QuickMenuRebornUnregisterWidget(id);
    }
    QuickMenuRebornUnregisterWidget(ID_PLANE_BUTTONS);
    QuickMenuRebornUnregisterWidget(ID_SECTION_TITLE);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);
}
