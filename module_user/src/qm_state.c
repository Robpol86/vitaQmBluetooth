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
 * @brief TODO.
 ******************************************************************************/

#include "qm_state.h"

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr/thread.h>
#include <quickmenureborn/c_types.h>
#include <quickmenureborn/qm_reborn.h>

#include "log.h"
#include "mutex.h"
#include "qm_ids.h"

#define BUTTON_LABEL_MAX (VQMBT_DEVICE_NAME_MAX + 16)

static SceKernelLwMutexWork mutex;

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

/**
 * TODO
 *
 * Called from _ thread TODO.
 */
static void refresh_ui(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        QmButton* qm_button = &qm_state.buttons[idx];
        VqmbtDeviceInfo* device = &qm_button->device;
        bool button_enabled = false;

        // Update label.
        char label[BUTTON_LABEL_MAX];
        if (device->mac0 == 0 && device->mac1 == 0) {
            sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        } else if (!qm_state.bluetooth_on) {
            sceClibSnprintf(label, sizeof(label), "Bluetooth is disabled");
        } else {
            switch (qm_button->state) {
                case BTNSTATE_DISCONNECTED:
                    sceClibSnprintf(label, sizeof(label), "Connect %s", device->name);
                    button_enabled = true;
                    break;
                case BTNSTATE_DISCONNECTING:
                    sceClibSnprintf(label, sizeof(label), "Disconnecting %s", device->name);
                    break;
                case BTNSTATE_CONNECTED:
                    sceClibSnprintf(label, sizeof(label), "Disconnect %s", device->name);
                    button_enabled = true;
                    break;
                case BTNSTATE_CONNECTING:
                    sceClibSnprintf(label, sizeof(label), "Connecting %s", device->name);
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
        } else {
            // Disable button.
            qm_button->enabled = false;
            QuickMenuRebornSetWidgetColor(id, 0.5F, 0.5F, 0.5F, 1.0F);
        }
    }
}

/**
 * TODO
 *
 * TODO:
 * - Refactor into state machine manager.
 *
 * Called from:
 *  - quickmenu_on_press (main thread)
 *  - handle_event and handle_event_dropped (event thread)
 *  - reset (event thread)
 *
 * @param request TODO
 */
void qm_state_update_ui(const QmStateRequest* request) {
    // Lock mutex and defer unlock to function scope exit.
    SceKernelLwMutexWork* mutex_ MUTEX_STATE = &mutex;  // TODO move into reset() and on_press(). Or maybe not.
    ENTER_MUTEX(mutex);

    bool changed = false;

    switch (request->id) {
        case REQUEST_BULK_UPDATE: {
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
                VqmbtDeviceInfo* old_device = &qm_button->device;
                // Detect new device in old slot or new state for existing device.
                if (new_device->mac0 == old_device->mac0 && new_device->mac1 == old_device->mac1) {
                    if (new_device->state == old_device->state) {
                        // TODO BUG stuck in Disconnecting state.
                        LOG_DEBUG(0, "No changes to \"%s\"", old_device->name);
                        continue;
                    }
                    LOG_DEBUG(0, "Device \"%s\" changed state from %d to %d", old_device->name, old_device->state,
                              new_device->state);
                    old_device->state = new_device->state;
                } else {
                    LOG_DEBUG(0, "New device detected in slot %d: \"%s\" (was \"%s\")", idx + 1, new_device->name,
                              old_device->name);
                    sceClibMemcpy(old_device, new_device, sizeof(*new_device));
                }
                changed = true;
                switch (old_device->state) {
                    case VQMBT_BT_STATE_DISCONNECTED:
                        qm_button->state = BTNSTATE_DISCONNECTED;
                        break;
                    case VQMBT_BT_STATE_CONNECTING:
                        qm_button->state = BTNSTATE_CONNECTING;
                        break;
                    case VQMBT_BT_STATE_DISCONNECTING:
                        qm_button->state = BTNSTATE_DISCONNECTING;
                        break;
                    case VQMBT_BT_STATE_REGISTERING:
                        LOG_DEBUG(0, "Handling registering state as connected for device \"%s\"", old_device->name);
                    case VQMBT_BT_STATE_CONNECTED:
                        qm_button->state = BTNSTATE_CONNECTED;
                        break;
                    default:
                        LOG_WARN("Unhandled state=%d for device \"%s\"", old_device->state, old_device->name);
                        qm_button->state = BTNSTATE_DISCONNECTED;
                        break;
                }
            }
            break;
        }

        case REQUEST_BUTTON_PRESSED: {
            QmButton* qm_button = &qm_state.buttons[request->idx];
            if (!qm_button->enabled) {
                LOG_DEBUG(0, "Button idx=%d pressed but disabled, ignoring", request->idx);
                break;
            }
            VqmbtDeviceInfo* device = &qm_button->device;
            switch (qm_button->state) {
                case BTNSTATE_DISCONNECTED:
                    LOG_DEBUG(0, "Connecting device \"%s\"", device->name);
                    kvqmbt_connect_device(device->mac0, device->mac1);
                    qm_button->state = BTNSTATE_CONNECTING;
                    changed = true;
                    break;
                case BTNSTATE_CONNECTED:
                    LOG_DEBUG(0, "Disconnecting device \"%s\"", device->name);
                    kvqmbt_disconnect_device(device->mac0, device->mac1);
                    qm_button->state = BTNSTATE_DISCONNECTING;
                    changed = true;
                    break;
                default:
                    LOG_DEBUG(0, "Ignoring state=%d for device \"%s\"", qm_button->state, device->name);
                    break;
            }
            // TODO show busy symbol or progress bar
            break;
        }

        case REQUEST_BLUETOOTH_ON: {
            if (qm_state.bluetooth_on) {
                LOG_DEBUG(0, "Bluetooth already displaying on");
            } else {
                LOG_DEBUG(0, "Displaying bluetooth as on");
                qm_state.bluetooth_on = true;
                changed = true;
            }
            break;
        }

        case REQUEST_BLUETOOTH_OFF: {
            if (!qm_state.bluetooth_on) {
                LOG_DEBUG(0, "Bluetooth already displaying off");
            } else {
                LOG_DEBUG(0, "Displaying bluetooth as off");
                qm_state.bluetooth_on = false;
                changed = true;
            }
            break;
        }

        case REQUEST_DEVICE_DISCONNECTED: {
            for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
                QmButton* qm_button = &qm_state.buttons[idx];
                VqmbtDeviceInfo* device = &qm_button->device;
                if (device->mac0 == request->mac.mac0 && device->mac1 == request->mac.mac1) {
                    LOG_DEBUG(0, "Device \"%s\" now disconnected", device->name);
                    qm_button->state = BTNSTATE_DISCONNECTED;
                    changed = true;
                    break;
                }
            }
            break;
        }

        case REQUEST_DEVICE_CONNECTED: {
            for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
                // TODO try connecting when Settings is opened.
                QmButton* qm_button = &qm_state.buttons[idx];
                VqmbtDeviceInfo* device = &qm_button->device;
                if (device->mac0 == request->mac.mac0 && device->mac1 == request->mac.mac1) {
                    LOG_DEBUG(0, "Device \"%s\" now connected", device->name);
                    qm_button->state = BTNSTATE_CONNECTED;
                    changed = true;
                    break;
                }
            }
            break;
        }
    }

    // Refresh UI.
    if (changed) {
        LOG_DEBUG(0, "Refreshing UI to show changes");
        refresh_ui();
    }
}

/**
 * TODO
 */
int qm_state_start(void) {
    // Initialize mutex.
    int ret = sceKernelCreateLwMutex(&mutex, "vqmbt-qm_state-mutex", 0, 0, NULL);
    if (ret < 0) {
        LOG_ERROR("sceKernelCreateLwMutex returned error 0x%08X", ret);
        return VQMBT_ERROR_GENERAL_FAILURE;
    }

    return 0;
}

/**
 * Unloads the plugin's quick menu items.
 */
int qm_state_stop(void) {
    // Delete mutex.
    int ret = sceKernelDeleteLwMutex(&mutex);
    LOG_DEBUG(0, "sceKernelDeleteLwMutex returned 0x%08X", ret);

    return 0;
}
