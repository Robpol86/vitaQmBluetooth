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
 * - Two body planes: one for the buttons, another with just a text message.
 * - Hide empty slots and resize plane to eliminate ghost scrolling.
 * TODO:
 * - callback: relabel button with new state. Surface error in button as close/reopen resets labels
 * - long bt names ellipses
 * - force close quickmenu. Clean up on recovery?
 * - update screenshot
 * - prune includes
 */

#include "quickmenu.h"

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr/thread.h>
#include <quickmenureborn/c_types.h>
#include <quickmenureborn/qm_reborn.h>
#include <stdbool.h>

#include "kmod_event.h"
#include "log.h"
#include "qm_ids.h"
#include "qm_state.h"
#include "vqmbt.h"

#define PREFIX "VqmbtEvent: "
#define INDENT "            "
_Static_assert(sizeof(PREFIX) == sizeof(INDENT), "INDENT width must match PREFIX");

/**
 * TODO
 *
 * Called from _ thread TODO.
 */
static void reset(void) {
    // Flush kernel buffer.
    int ret = kvqmbt_read_event(NULL, 0);
    if (ret < 0) {
        LOG_ERROR("kvqmbt_read_event returned error 0x%08X", ret);
    }

    // Get bluetooth state.
    bool bluetooth_on = kvqmbt_bluetooth_state();

    // Get all currently paired/registered bluetooth devices.
    VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES];
    sceClibMemset(devices, 0, sizeof(devices));
    int count = kvqmbt_get_paired_devices(devices, VQMBT_MAX_DEVICES);
    if (count < 0) {
        LOG_ERROR("kvqmbt_get_paired_devices returned error 0x%08X", count);
        return;
    }

    // Update the UI state and refresh the UI if changes were detected.
    qm_state_update_ui(&(QmsRequest){
        .id = QMS_REQUEST_BULK_UPDATE,
        .bulk.bluetooth_on = bluetooth_on,
        .bulk.num_devices = count,
        .bulk.devices = devices,
    });
}

/**
 * Handle scenario where one or more events went missing.
 *
 * Called from event thread only.
 */
static void handle_event_dropped(void) {
    LOG_DEBUG(0, "Running reset()");
    reset();
}

/**
 * Handler for one event. Called once per bluetooth event.
 *
 * Called from event thread only.
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
            reset();
            // qm_state_update_ui(&(QmsRequest){.id = QMS_REQUEST_BLUETOOTH_ON});  // TODO remove after deciding.
            break;

        case VQMBT_EVENT_BLUETOOTH_DISABLED:
            LOG_DEBUG(0, INDENT "Bluetooth turned off");
            qm_state_update_ui(&(QmsRequest){.id = QMS_REQUEST_BLUETOOTH_OFF});
            break;

        case VQMBT_EVENT_DEVICE_ADDED_REMOVED_CONNECTING:
            LOG_DEBUG(0, INDENT "Device added/removed/connecting");
            reset();
            break;

        case VQMBT_EVENT_DEVICE_DISCONNECTED:
            LOG_DEBUG(0, INDENT "Device disconnected");
            qm_state_update_ui(
                &(QmsRequest){.id = QMS_REQUEST_DEVICE_DISCONNECTED, .mac.mac0 = event->mac0, .mac.mac1 = event->mac1});
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_SUCCESS:
            LOG_DEBUG(0, INDENT "Device connected");
            qm_state_update_ui(
                &(QmsRequest){.id = QMS_REQUEST_DEVICE_CONNECTED, .mac.mac0 = event->mac0, .mac.mac1 = event->mac1});
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_FAILED:
            LOG_DEBUG(0, INDENT "Device connect failed");
            // TODO tell user it failed
            qm_state_update_ui(
                &(QmsRequest){.id = QMS_REQUEST_DEVICE_DISCONNECTED, .mac.mac0 = event->mac0, .mac.mac1 = event->mac1});
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_ABORTED:
            LOG_DEBUG(0, INDENT "Device connect aborted");
            // TODO tell user?
            qm_state_update_ui(
                &(QmsRequest){.id = QMS_REQUEST_DEVICE_DISCONNECTED, .mac.mac0 = event->mac0, .mac.mac1 = event->mac1});
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_CANCELLED:
            LOG_DEBUG(0, INDENT "Device connect cancelled");
            qm_state_update_ui(
                &(QmsRequest){.id = QMS_REQUEST_DEVICE_DISCONNECTED, .mac.mac0 = event->mac0, .mac.mac1 = event->mac1});
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
 * Called from main thread only.
 *
 * TODO:
 * - Relabel button "Connecting <name>...".
 * - when user taps a button disable all buttons and wait for callback.
 * TODO:
 * - If this button has been relabeled 500ms ago noop the tap event (ignore taps on other buttons)
 */
static BUTTON_HANDLER(quickmenu_on_press) {
    (void)id;
    (void)hash;
    (void)eventId;
    int idx = (int)(intptr_t)userDat;

    LOG_DEBUG(0, "Button idx=%d pressed", idx);
    qm_state_update_ui(&(QmsRequest){.id = QMS_REQUEST_BUTTON_PRESSED, .idx = idx});
}

/**
 * Called when the quick menu is opened by the user.
 */
static ONLOAD_HANDLER(quickmenu_on_load) {
    (void)id;

    LOG_DEBUG(0, "Quick menu opened.");

    // Start event thread.
    // TODO comment out, observe behavior. Does initial state look acceptable to users?
    kmod_event_start(reset, handle_event_dropped, handle_event);
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
 */
int quickmenu_start(void) {
    // Initialize mutex and state.
    qm_state_mutex_start();

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
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornRegisterWidget(id, ID_PLANE_BUTTONS, button);
        QuickMenuRebornSetWidgetSize(id, 600, 75, 0, 0);
        QuickMenuRebornSetWidgetPosition(id, 20, 280 - (idx * 80), 0, 0);
        QuickMenuRebornSetWidgetColor(id, 1, 1, 1, 1);
        QuickMenuRebornRegisterEventHanlder(id, QMR_BUTTON_RELEASE_ID, quickmenu_on_press, (void*)(intptr_t)idx);
    }

    // Register handlers.
    const char* last_widget = ID_BUTTONS[VQMBT_MAX_DEVICES - 1];
    QuickMenuRebornAssignOnLoadHandler(quickmenu_on_load, last_widget);
    QuickMenuRebornAssignOnDeleteHandler(quickmenu_on_unload, last_widget);

    return 0;
}

/**
 * Unloads the plugin's quick menu items.
 */
int quickmenu_stop(void) {
    // Stop event thread.
    kmod_event_stop();

    // Unregister widgets.
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornUnregisterWidget(id);
    }
    QuickMenuRebornUnregisterWidget(ID_PLANE_BUTTONS);
    QuickMenuRebornUnregisterWidget(ID_SECTION_TITLE);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);

    // Remove mutex.
    qm_state_mutex_stop();

    return 0;
}
