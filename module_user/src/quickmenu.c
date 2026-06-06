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
#include <stdbool.h>

#include "kmod_event.h"
#include "log.h"
#include "mutex.h"
#include "vqmbt.h"

#define BUTTON_LABEL_MAX (VQMBT_DEVICE_NAME_MAX + 16)

#define PREFIX "VqmbtEvent: "
#define INDENT "            "
_Static_assert(sizeof(PREFIX) == sizeof(INDENT), "INDENT width must match PREFIX");

static SceKernelLwMutexWork mutex;

// Widget IDs (prefixed because they must be unique across all plugins).
#define ID_SEPARATOR MODULE_NAME "Separator"
#define ID_SECTION_TITLE MODULE_NAME "SectionTitle"
#define ID_PLANE_BUTTONS MODULE_NAME "PlaneButtons"

// Button IDs.
static const char* const ID_BUTTONS[VQMBT_MAX_DEVICES] = {
    MODULE_NAME "Button0", MODULE_NAME "Button1", MODULE_NAME "Button2", MODULE_NAME "Button3",
    MODULE_NAME "Button4", MODULE_NAME "Button5", MODULE_NAME "Button6", MODULE_NAME "Button7",
};
_Static_assert(sizeof(ID_BUTTONS) / sizeof(ID_BUTTONS[0]) == VQMBT_MAX_DEVICES, "ID_BUTTONS size != VQMBT_MAX_DEVICES");

// Quickmenu and button states.
typedef enum QmButtonState : unsigned int {
    BTNSTATE_DISCONNECTED = 0,
    BTNSTATE_DISCONNECTING,
    BTNSTATE_CONNECTED,
    BTNSTATE_CONNECTING,
} QmButtonState;
typedef struct QmButton {
    VqmbtDeviceInfo device;
    QmButtonState state;
    bool enabled;
} QmButton;
typedef struct QmState {
    bool bluetooth_on;
    int num_buttons_active;
    QmButton buttons[VQMBT_MAX_DEVICES];
} QmState;
static QmState qm_state;

// UI request.
typedef enum QmRequestId : unsigned int {
    REQUEST_BULK_UPDATE = 0,
    REQUEST_BLUETOOTH_ON,
    REQUEST_BLUETOOTH_OFF,
} QmRequestId;
typedef struct QmRequest {
    QmRequestId id;
    union {
        int idx;
        struct {
            unsigned int mac0, mac1;
        } mac;
        struct {
            bool bluetooth_on;
            int num_devices;
            const VqmbtDeviceInfo* devices;
        } bulk;
    };
} QmRequest;

/**
 * TODO
 *
 * Called from _ thread TODO.
 */
static void refresh_ui(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        QmButton* qm_button = &qm_state.buttons[idx];
        bool button_enabled = false;

        // Update label.
        char label[BUTTON_LABEL_MAX];
        if (qm_button->device.mac0 == 0 && qm_button->device.mac1 == 0) {
            sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        } else if (!qm_state.bluetooth_on) {
            sceClibSnprintf(label, sizeof(label), "Bluetooth is disabled");
        } else {
            switch (qm_button->state) {
                case BTNSTATE_DISCONNECTED:
                    sceClibSnprintf(label, sizeof(label), "Connect %s", qm_button->device.name);
                    button_enabled = true;
                    break;
                case BTNSTATE_DISCONNECTING:
                    sceClibSnprintf(label, sizeof(label), "Disconnecting %s", qm_button->device.name);
                    break;
                case BTNSTATE_CONNECTED:
                    sceClibSnprintf(label, sizeof(label), "Disconnect %s", qm_button->device.name);
                    button_enabled = true;
                    break;
                case BTNSTATE_CONNECTING:
                    sceClibSnprintf(label, sizeof(label), "Connecting %s", qm_button->device.name);
                    break;
            }
        }
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornSetWidgetLabel(id, label);

        // Enable/disable button.
        if (qm_state.bluetooth_on && button_enabled) {
            // Enable button.
            qm_button->enabled = true;
            QuickMenuRebornSetWidgetColor(id, 1.0F, 1.0F, 1.0F, 1.0F);
            // TODO noop callback
        } else {
            // Disable button.
            qm_button->enabled = false;
            QuickMenuRebornSetWidgetColor(id, 0.5F, 0.5F, 0.5F, 1.0F);
            // TODO noop callback
        }
    }
}

/**
 * TODO
 *
 * Called from _ thread TODO.
 */
static void update_ui(const QmRequest* request) {
    // Lock mutex and defer unlock to function scope exit.
    SceKernelLwMutexWork* mutex_ MUTEX_STATE = &mutex;  // TODO move into reset() and on_press().
    ENTER_MUTEX(mutex);

    bool changed = false;

    switch (request->id) {
        case REQUEST_BULK_UPDATE:
            if (request->bulk.bluetooth_on != qm_state.bluetooth_on) {
                LOG_DEBUG(0, "Bluetooth toggled");
                qm_state.bluetooth_on = request->bulk.bluetooth_on;
                changed = true;
            }
            if (request->bulk.num_devices != qm_state.num_buttons_active) {
                LOG_DEBUG(0, "Number of active buttons went from %d to %d", qm_state.num_buttons_active,
                          request->bulk.num_devices);
                qm_state.num_buttons_active = request->bulk.num_devices;
                changed = true;
            }
            for (int idx = 0; idx < request->bulk.num_devices; idx++) {
                const VqmbtDeviceInfo* new_device = &request->bulk.devices[idx];
                QmButton* qm_button = &qm_state.buttons[idx];
                // Detect new device in old slot or new state for existing device.
                if (new_device->mac0 == qm_button->device.mac0 && new_device->mac1 == qm_button->device.mac1) {
                    if (new_device->state == qm_button->device.state) {
                        LOG_DEBUG(0, "No changes to \"%s\"", qm_button->device.name);
                        continue;
                    }
                    LOG_DEBUG(0, "Device \"%s\" changed state from %d to %d", qm_button->device.name,
                              qm_button->device.state, new_device->state);
                    qm_button->device.state = new_device->state;
                } else {
                    LOG_DEBUG(0, "New device detected in slot %d: \"%s\" (was \"%s\")", idx + 1, new_device->name,
                              qm_button->device.name);
                    sceClibMemcpy(&qm_button->device, new_device, sizeof(*new_device));
                }
                changed = true;
                switch (qm_button->device.state) {
                    case VQMBT_BT_STATE_DISCONNECTED:
                        qm_button->state = BTNSTATE_DISCONNECTED;
                        break;
                    case VQMBT_BT_STATE_CONNECTING:
                        qm_button->state = BTNSTATE_CONNECTING;
                        break;
                    case VQMBT_BT_STATE_DISCONNECTING:
                        qm_button->state = BTNSTATE_DISCONNECTING;
                        break;
                    case VQMBT_BT_STATE_CONNECTED:
                        qm_button->state = BTNSTATE_CONNECTED;
                        break;
                    case VQMBT_BT_STATE_REGISTERING:
                        qm_button->state = BTNSTATE_CONNECTING;
                        break;
                    default:
                        LOG_WARN("Unhandled state=%d for device \"%s\"", qm_button->device.state, qm_button->device.name);
                        qm_button->state = BTNSTATE_DISCONNECTED;
                        break;
                }
            }
            break;

        case REQUEST_BLUETOOTH_ON:
            if (qm_state.bluetooth_on) {
                LOG_DEBUG(0, "Bluetooth already displaying on");
            } else {
                LOG_DEBUG(0, "Displaying bluetooth as on");
                qm_state.bluetooth_on = true;
                changed = true;
            }
            break;

        case REQUEST_BLUETOOTH_OFF:
            if (!qm_state.bluetooth_on) {
                LOG_DEBUG(0, "Bluetooth already displaying off");
            } else {
                LOG_DEBUG(0, "Displaying bluetooth as off");
                qm_state.bluetooth_on = false;
                changed = true;
            }
            break;
    }

    // Refresh UI.
    if (changed) {
        refresh_ui();
    }
}

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
    update_ui(&(QmRequest){
        .id = REQUEST_BULK_UPDATE,
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
    reset();  // TODO test
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
            update_ui(&(QmRequest){.id = REQUEST_BLUETOOTH_ON});
            break;

        case VQMBT_EVENT_BLUETOOTH_DISABLED:
            LOG_DEBUG(0, INDENT "Bluetooth turned off");
            update_ui(&(QmRequest){.id = REQUEST_BLUETOOTH_OFF});
            break;

        case VQMBT_EVENT_DEVICE_ADDED_REMOVED_CONNECTING:
            LOG_DEBUG(0, INDENT "Device added/removed/connecting");
            // TODO reset()
            break;

        case VQMBT_EVENT_DEVICE_DISCONNECTED:
            LOG_DEBUG(0, INDENT "Device disconnected");
            // TODO update_ui(new_state=VQMBT_BT_STATE_DISCONNECTED, mac0, mac1)
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_SUCCESS:
            LOG_DEBUG(0, INDENT "Device connected");
            // TODO update_ui(new_state=VQMBT_BT_STATE_CONNECTED, mac0, mac1)
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_FAILED:
            LOG_DEBUG(0, INDENT "Device connect failed");
            // TODO tell user it failed
            // TODO update_ui(new_state=VQMBT_BT_STATE_DISCONNECTED, mac0, mac1)
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_ABORTED:
            LOG_DEBUG(0, INDENT "Device connect aborted");
            // TODO tell user?
            // TODO update_ui(new_state=VQMBT_BT_STATE_DISCONNECTED, mac0, mac1)
            break;

        case VQMBT_EVENT_DEVICE_CONNECT_CANCELLED:
            LOG_DEBUG(0, INDENT "Device connect cancelled");
            // TODO update_ui(new_state=VQMBT_BT_STATE_DISCONNECTED, mac0, mac1)
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

    QmButton* qm_button = &qm_state.buttons[idx];

    if (!qm_button->enabled) {
        LOG_DEBUG(0, "Button idx=%d pressed but disabled, ignoring", idx);
        return;
    }

    LOG_DEBUG(0, "Button idx=%d pressed", idx);

    // TODO update_ui(action=idx)
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

    // Start event thread.
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
int quickmenu_start(void) {
    // Initialize mutex.
    int ret = sceKernelCreateLwMutex(&mutex, "vqmbt-quickmenu-mutex", 0, 0, NULL);
    if (ret < 0) {
        LOG_ERROR("sceKernelCreateLwMutex returned error 0x%08X", ret);
        return VQMBT_ERROR_GENERAL_FAILURE;
    }

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
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornUnregisterWidget(id);
    }
    QuickMenuRebornUnregisterWidget(ID_PLANE_BUTTONS);
    QuickMenuRebornUnregisterWidget(ID_SECTION_TITLE);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);

    int ret = sceKernelDeleteLwMutex(&mutex);
    LOG_DEBUG(0, "sceKernelDeleteLwMutex returned 0x%08X", ret);

    return 0;
}
