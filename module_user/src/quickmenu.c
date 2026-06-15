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
 * - If kernel plugin isn't loaded notify user.
 * - If kernel plugin loaded AFTER user plugin what happens? probably fails. Handle this too.
 * - If kernel plugin's version does not match user module's fail with error (surfaced in UI or Settings).
 * - Two body planes: one for the buttons, another with just a text message.
 * - Hide empty slots and resize plane to eliminate ghost scrolling.
 * - long bt names ellipses
 * - force close quickmenu. Clean up on recovery?
 */

#include "quickmenu.h"

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <quickmenureborn/qm_reborn.h>

#include "log.h"
#include "qm_ids.h"
#include "vqmbt.h"

#define BUTTON_LABEL_MAX (VQMBT_SCE_DEVICE_NAME_MAX + 16)

static VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES];  // TODO locking/semaphore?

#define PREFIX "VqmbtEvent: "
#define INDENT "            "
_Static_assert(sizeof(PREFIX) == sizeof(INDENT), "INDENT width must match PREFIX");

/**
 * Handle scenario where one or more events went missing.
 */
static void handle_event_dropped(void) {
    // TODO
    LOG_DEBUG(0, "TODO re-run kvqmbt_get_paired_devices()");
}

/**
 * Handler for one event. Called once per bluetooth event.
 *
 * @param event Event details.
 */
static void handle_event(const VqmbtEvent* event) {
    LOG_DEBUG(0, PREFIX "id=0x%08X mac0=0x%08X mac1=0x%08X", event->id, event->mac0, event->mac1);

    // Handle events.
    switch (event->id) {
        case VQMBT_EVENT_DROPPED_EVENTS:
            LOG_DEBUG(0, INDENT "Missing bluetooth events detected");
            handle_event_dropped();
            break;

        case VQMBT_EVENT_BLUETOOTH_ENABLED:
            LOG_DEBUG(0, INDENT "Bluetooth turned on");
            break;

        case VQMBT_EVENT_BLUETOOTH_DISABLED:
            LOG_DEBUG(0, INDENT "Bluetooth turned off");
            break;

        case VQMBT_EVENT_DEVICE_ADDED_REMOVED_CONNECTING:
            LOG_DEBUG(0, INDENT "Device added/removed/connecting");
            break;

        case VQMBT_EVENT_DEVICE_DISCONNECTED:
            LOG_DEBUG(0, INDENT "Device disconnected");
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_SUCCESS:
            LOG_DEBUG(0, INDENT "Device connected");
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_FAILED:
            LOG_DEBUG(0, INDENT "Device connect failed");
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_ABORTED:
            LOG_DEBUG(0, INDENT "Device connect aborted");
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_CANCELLED:
            LOG_DEBUG(0, INDENT "Device connect cancelled");
            break;

        default:
            LOG_WARN(INDENT "Unhandled event id=0x%08X", event->id);
            LOG_DEBUG(0, INDENT "Ignoring id=0x%08X", event->id);
            break;
    }
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

    VqmbtDeviceInfo* dev = &devices[idx];

    // Do nothing if slot is empty.
    if ((dev->mac0 | dev->mac1) == 0) {  // TODO make more robust, use `static int devices_count`?
        LOG_DEBUG(0, "button idx=%d is empty: no-op", idx);
        return;
    }

    if (dev->state == 5 || dev->state == 6) {
        LOG_DEBUG(0, "Disconnecting \"%s\"", dev->name);
        kvqmbt_disconnect_device(dev->mac0, dev->mac1);
    } else {
        LOG_DEBUG(0, "Connecting \"%s\"", dev->name);
        kvqmbt_connect_device(dev->mac0, dev->mac1);
    }
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

    // Zero the struct array to prevent ghost data.
    sceClibMemset(devices, 0, sizeof(devices));

    // Query kernel.
    VqmbtDeviceInfo* dev;
    int count = kvqmbt_get_paired_devices(devices, VQMBT_MAX_DEVICES);
    if (count < 0) {
        LOG_ERROR("kvqmbt_get_paired_devices returned error 0x%08X", count);
        return;
    }
    if (count < 1) {
        LOG_DEBUG(0, "No paired devices.");
        return;
    }
    LOG_DEBUG(0, "count=%d", count);

    // Update button labels.
    for (int idx = 0; idx < count; idx++) {
        dev = &devices[idx];
        LOG_DEBUG(0, "idx=%d name=\"%s\" mac0=0x%08X mac1=0x%08X", idx, dev->name, dev->mac0, dev->mac1);
        const char* id = QM_ID_BUTTONS[idx];
        char label[BUTTON_LABEL_MAX];
        if (dev->state == 5 || dev->state == 6) {  // TODO dedupe magic numbers with enum?
            sceClibSnprintf(label, sizeof(label), "Disconnect %s", dev->name);
        } else {
            sceClibSnprintf(label, sizeof(label), "Connect %s", dev->name);
        }
        QuickMenuRebornSetWidgetLabel(id, label);
    }
}

/**
 * Called when the quick menu is closed by the user.
 */
static void quickmenu_on_unload(const char* id) {
    (void)id;

    LOG_DEBUG(0, "Quick menu closed.");

    // Reset button labels.
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = QM_ID_BUTTONS[idx];
        char label[BUTTON_LABEL_MAX];
        sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        QuickMenuRebornSetWidgetLabel(id, label);
    }
}

/**
 * Loads the plugin's quick menu items.
 *
 * TODO:
 * - callback: relabel button with new state. Surface error in button as close/reopen resets labels
 * - button_reset() button_disable() button_enable() functions
 * TODO:
 * - Add function to calculate position from top left instead of center.
 * - Pixel perfect alignment.
 */
void quickmenu_start(void) {
    // Add horizontal line separator.
    QuickMenuRebornSeparator(QM_ID_SEPARATOR, SCE_SEPARATOR_HEIGHT);

    // Add section heading text.
    QuickMenuRebornRegisterWidget(QM_ID_SECTION_TITLE, NULL, text);
    QuickMenuRebornSetWidgetSize(QM_ID_SECTION_TITLE, 300, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(QM_ID_SECTION_TITLE, -206, 0, 0, 0);
    QuickMenuRebornSetWidgetColor(QM_ID_SECTION_TITLE, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(QM_ID_SECTION_TITLE, "Bluetooth Devices");

    // Add the button plane that holds all the buttons.
    QuickMenuRebornRegisterWidget(QM_ID_PLANE_BUTTONS, NULL, plane);
    QuickMenuRebornSetWidgetSize(QM_ID_PLANE_BUTTONS, SCE_PLANE_WIDTH, 650, 0, 0);
    QuickMenuRebornSetWidgetColor(QM_ID_PLANE_BUTTONS, 1, 1, 1, 0);

    // Add device buttons.
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = QM_ID_BUTTONS[idx];
        QuickMenuRebornRegisterWidget(id, QM_ID_PLANE_BUTTONS, button);
        QuickMenuRebornSetWidgetSize(id, 600, 75, 0, 0);
        QuickMenuRebornSetWidgetPosition(id, 20, 280 - (idx * 80), 0, 0);
        QuickMenuRebornSetWidgetColor(id, 1, 1, 1, 1);
        char label[BUTTON_LABEL_MAX];
        sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        QuickMenuRebornSetWidgetLabel(id, label);
        QuickMenuRebornRegisterEventHanlder(id, QMR_BUTTON_RELEASE_ID, quickmenu_on_press, (void*)(intptr_t)idx);
    }

    // Register handlers.
    const char* last_widget = QM_ID_BUTTONS[VQMBT_MAX_DEVICES - 1];
    QuickMenuRebornAssignOnLoadHandler(quickmenu_on_load, last_widget);
    QuickMenuRebornAssignOnDeleteHandler(quickmenu_on_unload, last_widget);
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = QM_ID_BUTTONS[idx];
        QuickMenuRebornUnregisterWidget(id);
    }
    QuickMenuRebornUnregisterWidget(QM_ID_PLANE_BUTTONS);
    QuickMenuRebornUnregisterWidget(QM_ID_SECTION_TITLE);
    QuickMenuRebornRemoveSeparator(QM_ID_SEPARATOR);
}
