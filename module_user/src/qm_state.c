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

/**
 * TODO:
 * - Have event thread constantly get states for all or select devices whilst QM is open?
 * - Test QM updating when Settings app connects/disconnects/enables/disables/add/remove
 * - BUG: Connect device, then turn off BT. Abort event is emitted and button is relabled to Connect.
 * - Keep flat, no btoff var. bt on state guarded. only bulk/bt_off can unset it. only bulk/bt_on can set.
 * - TODO: transition table?
 * - Show error message in button, e.g. when bt syscall is busy
 * - Make bulk_update resilient.

Transition rules:

+--------------+-----------------------+---------------------------------+--------+
| Condition    | Source State          | Target State                    | Result |
+--------------+-----------------------+---------------------------------+--------+
| Bulk Update  | *                     | *                               | accept |
| Add Device   | Source                | Target                          |        |
| Event Thread |                       |                                 |        |
| Button Press | BTNSTATE_DISCONNECTED | BTNSTATE_CONNECTING_DISABLED    | accept |
| Button Press | BTNSTATE_CONNECTED    | BTNSTATE_DISCONNECTING_DISABLED | accept |
| *            | *                     | *                               | no-op  |
+--------------+-----------------------+---------------------------------+--------+

- any -> BTNSTATE_SLOT_EMPTY_DISABLED
- !BTNSTATE_SLOT_EMPTY_DISABLED -> BTNSTATE_BT_OFF_DISABLED
- !BTNSTATE_BT_OFF_DISABLED -> BTNSTATE_DISCONNECTED
- !BTNSTATE_BT_OFF_DISABLED -> BTNSTATE_DISCONNECTING_DISABLED
- !BTNSTATE_BT_OFF_DISABLED -> BTNSTATE_CONNECTED
- !BTNSTATE_BT_OFF_DISABLED -> BTNSTATE_CONNECTING_DISABLED
 */

#include "qm_state.h"

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr/thread.h>
#include <quickmenureborn/c_types.h>
#include <quickmenureborn/qm_reborn.h>

#include "log.h"
#include "mutex.h"
#include "qm_ids.h"
#include "vqmbt.h"

#define BUTTON_LABEL_MAX (VQMBT_DEVICE_NAME_MAX + 16)

static SceKernelLwMutexWork mutex;

// Button finite states.
typedef enum QmButtonState : unsigned int {
    BTNSTATE_SLOT_EMPTY_DISABLED,
    BTNSTATE_BT_OFF_DISABLED,
    BTNSTATE_DISCONNECTED,
    BTNSTATE_DISCONNECTING_DISABLED,
    BTNSTATE_CONNECTED,
    BTNSTATE_CONNECTING_DISABLED,
} QmButtonState;

// Button descriptor.
typedef struct QmButton {
    VqmbtDeviceInfo device;
    QmButtonState state;
} QmButton;

// Quickmenu descriptor.
typedef struct QmState {
    QmButton buttons[VQMBT_MAX_DEVICES];
} QmState;

// Quickmenu state.
static QmState qm_state;

/**
 * TODO
 *
 * Called from _ thread TODO.
 */
static void refresh_ui(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const QmButton* qm_button = &qm_state.buttons[idx];
        const VqmbtDeviceInfo* device = &qm_button->device;
        char label[BUTTON_LABEL_MAX];
        bool button_enabled = false;

        // Determine label.
        switch (qm_button->state) {
            case BTNSTATE_SLOT_EMPTY_DISABLED:
                sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
                break;
            case BTNSTATE_BT_OFF_DISABLED:
                sceClibSnprintf(label, sizeof(label), "Bluetooth is disabled");
                break;
            case BTNSTATE_DISCONNECTING_DISABLED:
                sceClibSnprintf(label, sizeof(label), "Disconnecting %s", device->name);
                break;
            case BTNSTATE_CONNECTING_DISABLED:
                sceClibSnprintf(label, sizeof(label), "Connecting %s", device->name);
                break;
            case BTNSTATE_DISCONNECTED:
                sceClibSnprintf(label, sizeof(label), "Connect %s", device->name);
                button_enabled = true;
                break;
            case BTNSTATE_CONNECTED:
                sceClibSnprintf(label, sizeof(label), "Disconnect %s", device->name);
                button_enabled = true;
                break;
        }

        // Update label.
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornSetWidgetLabel(id, label);

        // Enable/disable button.
        if (button_enabled) {
            // Enable button.
            QuickMenuRebornSetWidgetColor(id, 1.0F, 1.0F, 1.0F, 1.0F);
        } else {
            // Disable button.
            QuickMenuRebornSetWidgetColor(id, 0.5F, 0.5F, 0.5F, 1.0F);
        }
    }
}

/**
 * Transtion to BTNSTATE_SLOT_EMPTY_DISABLED.
 */
static void transition_state_unoccupied(bool* changed, const int idx) {
    QmButton* qm_button = &qm_state.buttons[idx];
    if (qm_button->state != BTNSTATE_SLOT_EMPTY_DISABLED) {
        LOG_DEBUG(0, "Setting slot %d as empty", idx + 1);
        sceClibMemset(&qm_button->device, 0, sizeof(qm_button->device));
        qm_button->state = BTNSTATE_SLOT_EMPTY_DISABLED;
        *changed = true;
        return;
    }
    LOG_DEBUG(0, "No-op");
}

/**
 * Transition to BTNSTATE_BT_OFF_DISABLED.
 */
static void transition_state_bt_off(bool* changed, const int* idx) {
    // TODO remove idx and toggle qm_state.bluetooth_off
    if (idx == NULL) {
        LOG_DEBUG(0, "Setting all occupied slots as bluetooth off");
        for (int i = 0; i < VQMBT_MAX_DEVICES; i++) {
            transition_state_bt_off(changed, &i);
        }
        return;
    }
    QmButton* qm_button = &qm_state.buttons[*idx];
    if (qm_button->state != BTNSTATE_BT_OFF_DISABLED && qm_button->state != BTNSTATE_SLOT_EMPTY_DISABLED) {
        LOG_DEBUG(0, "Setting slot %d as bluetooth off", *idx + 1);
        qm_button->state = BTNSTATE_BT_OFF_DISABLED;
        *changed = true;
        return;
    }
    LOG_DEBUG(0, "No-op");
}

/**
 * TODO.
 */
static void transition_state_new_device(bool* changed, const int idx, const VqmbtDeviceInfo* new_device) {
    QmButton* qm_button = &qm_state.buttons[idx];
    VqmbtDeviceInfo* old_device = &qm_button->device;
    if (new_device->mac0 == old_device->mac0 && new_device->mac1 == old_device->mac1) return;

    LOG_DEBUG(0, "New device in slot %d: \"%s\" (was \"%s\")", idx + 1, new_device->name, old_device->name);
    sceClibMemcpy(old_device, new_device, sizeof(*new_device));

    switch (old_device->state) {
        case VQMBT_BT_STATE_DISCONNECTED:
            qm_button->state = BTNSTATE_DISCONNECTED;
            break;
        case VQMBT_BT_STATE_CONNECTING:
            qm_button->state = BTNSTATE_CONNECTING_DISABLED;
            break;
        case VQMBT_BT_STATE_DISCONNECTING:
            qm_button->state = BTNSTATE_DISCONNECTING_DISABLED;
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

    *changed = true;
}

/**
 * Transition to BTNSTATE_DISCONNECTED.
 */
static void transition_state_disconnected(bool* changed, const int idx, bool force) {
    QmButton* qm_button = &qm_state.buttons[idx];
    if (qm_button->state == BTNSTATE_BT_OFF_DISABLED && !force) {
        LOG_DEBUG(0, "No-op");
        return;
    }
    if (qm_button->state != BTNSTATE_DISCONNECTED) {
        LOG_DEBUG(0, "Setting slot %d as disconnected", idx + 1);
        qm_button->state = BTNSTATE_DISCONNECTED;
        *changed = true;
        return;
    }
    LOG_DEBUG(0, "No-op");
}

/**
 * Transition to BTNSTATE_DISCONNECTING_DISABLED.
 */
static void transition_state_busy_disconnecting(bool* changed, const int idx) {
    QmButton* qm_button = &qm_state.buttons[idx];
    if (qm_button->state != BTNSTATE_DISCONNECTING_DISABLED) {
        LOG_DEBUG(0, "Setting slot %d as disconnecting", idx + 1);
        qm_button->state = BTNSTATE_DISCONNECTING_DISABLED;
        *changed = true;
        return;
    }
    LOG_DEBUG(0, "No-op");
}

/**
 * Transition to BTNSTATE_CONNECTED.
 */
static void transition_state_connected(bool* changed, const int idx) {
    QmButton* qm_button = &qm_state.buttons[idx];
    if (qm_button->state != BTNSTATE_CONNECTED && qm_button->state != BTNSTATE_BT_OFF_DISABLED) {
        LOG_DEBUG(0, "Setting slot %d as connected", idx + 1);
        qm_button->state = BTNSTATE_CONNECTED;
        *changed = true;
        return;
    }
    LOG_DEBUG(0, "No-op");
}

/**
 * Transition to BTNSTATE_CONNECTING_DISABLED.
 */
static void transition_state_busy_connecting(bool* changed, const int idx) {
    QmButton* qm_button = &qm_state.buttons[idx];
    if (qm_button->state != BTNSTATE_CONNECTING_DISABLED) {
        LOG_DEBUG(0, "Setting slot %d as connecting", idx + 1);
        qm_button->state = BTNSTATE_CONNECTING_DISABLED;
        *changed = true;
        return;
    }
    LOG_DEBUG(0, "No-op");
}

/**
 * TODO
 */
static void button_pressed(bool* changed, const int idx) {
    QmButton* qm_button = &qm_state.buttons[idx];
    const VqmbtDeviceInfo* device = &qm_button->device;

    switch (qm_button->state) {
        case BTNSTATE_SLOT_EMPTY_DISABLED:
        case BTNSTATE_BT_OFF_DISABLED:
        case BTNSTATE_DISCONNECTING_DISABLED:
        case BTNSTATE_CONNECTING_DISABLED:
            LOG_DEBUG(0, "Button idx=%d pressed but disabled, ignoring", idx);
            return;
        case BTNSTATE_DISCONNECTED:
            LOG_DEBUG(0, "Connecting device \"%s\"", device->name);
            kvqmbt_connect_device(device->mac0, device->mac1);  // TODO ret error
            transition_state_busy_connecting(changed, idx);
            break;
        case BTNSTATE_CONNECTED:
            LOG_DEBUG(0, "Disconnecting device \"%s\"", device->name);
            kvqmbt_disconnect_device(device->mac0, device->mac1);  // TODO ret error
            transition_state_busy_disconnecting(changed, idx);
            break;
    }
    // TODO show busy symbol or progress bar
}

/**
 * TODO
 */
static void transition_state_bt_on(bool* changed) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const QmButton* qm_button = &qm_state.buttons[idx];
        const VqmbtDeviceInfo* device = &qm_button->device;

        // Skip if empty.
        if (qm_button->state == BTNSTATE_SLOT_EMPTY_DISABLED) {
            LOG_DEBUG(0, "Skipping empty slot idx=%d", idx);
            continue;
        }

        LOG_DEBUG(0, "Restoring state for slot idx=%d", idx);

        // Restore state.  // TODO DRY.
        switch (device->state) {
            case VQMBT_BT_STATE_DISCONNECTED:
                transition_state_disconnected(changed, idx, false);
                break;
            case VQMBT_BT_STATE_CONNECTING:
                transition_state_busy_connecting(changed, idx);
                break;
            case VQMBT_BT_STATE_DISCONNECTING:
                transition_state_busy_disconnecting(changed, idx);
                break;
            case VQMBT_BT_STATE_CONNECTED:
            case VQMBT_BT_STATE_REGISTERING:
                transition_state_connected(changed, idx);
                break;
            default:
                transition_state_disconnected(changed, idx, false);
                break;
        }
    }
}

/**
 * TODO
 */
static void bulk_update(bool* changed, const QmsRequest* request) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        LOG_DEBUG(0, "Bulk updating idx=%d", idx);
        // Check if device was removed.
        if (idx >= request->bulk.num_devices) {
            LOG_DEBUG(0, "No device");
            transition_state_unoccupied(changed, idx);
            continue;
        }

        const VqmbtDeviceInfo* new_device = &request->bulk.devices[idx];
        const VqmbtDeviceInfo* old_device = &qm_state.buttons[idx].device;

        // Check if slot has a new device.
        if (new_device->mac0 != old_device->mac0 || new_device->mac1 != old_device->mac1) {
            LOG_DEBUG(0, "New device");
            transition_state_new_device(changed, idx, new_device);
            if (!request->bulk.bluetooth_on) {
                LOG_DEBUG(0, "New device but bluetooth is off");
                transition_state_bt_off(changed, &idx);
            }
            continue;
        }

        // Check if bluetooth is off.
        if (!request->bulk.bluetooth_on) {
            LOG_DEBUG(0, "Old device and bluetooth is off");
            transition_state_bt_off(changed, &idx);
            continue;
        }

        // New state.
        LOG_DEBUG(0, "Old device updating state");
        switch (new_device->state) {
            case VQMBT_BT_STATE_CONNECTING:
                transition_state_busy_connecting(changed, idx);
                break;
            case VQMBT_BT_STATE_DISCONNECTING:
                transition_state_busy_disconnecting(changed, idx);
                break;
            case VQMBT_BT_STATE_CONNECTED:
            case VQMBT_BT_STATE_REGISTERING:
                transition_state_connected(changed, idx);
                break;
            default:
                transition_state_disconnected(changed, idx, true);
                break;
        }
    }
}

/**
 * Look up the index of the button with the given MAC address. Returns -1 if not found or invalid MAC. TODO.
 *
 * @param mac0 TODO
 * @param mac1 TODO
 * @return TODO
 */
static int mac_to_idx(const unsigned int mac0, const unsigned int mac1) {
    if (mac0 < 1 || mac1 < 1) {
        LOG_ERROR("Invalid MAC address: mac0=0x%08X mac1=0x%08X", mac0, mac1);
        return VQMBT_ERROR_INVALID_ARGUMENT;
    }
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const VqmbtDeviceInfo* device = &qm_state.buttons[idx].device;
        if (device->mac0 == mac0 && device->mac1 == mac1) {
            return idx;
        }
    }
    LOG_ERROR("MAC address not found: mac0=0x%08X mac1=0x%08X", mac0, mac1);
    return VQMBT_ERROR_INVALID_ARGUMENT;
}

/**
 * TODO
 *
 * Called from:
 *  - quickmenu_on_press (main thread)
 *  - handle_event and handle_event_dropped (event thread)
 *  - reset (event thread)
 *
 * @param request TODO
 */
void qm_state_update_ui(const QmsRequest* request) {
    // Lock mutex and defer unlock to function scope exit.
    SceKernelLwMutexWork* mutex_ MUTEX_STATE = &mutex;  // TODO move back here and eliminate mutex.h.
    ENTER_MUTEX(mutex);

    bool changed = false;

    switch (request->id) {
        case QMS_REQUEST_BULK_UPDATE: {
            LOG_DEBUG(0, "Request bulk update %d devices", request->bulk.num_devices);
            bulk_update(&changed, request);
            break;
        }

        case QMS_REQUEST_BUTTON_PRESSED: {
            LOG_DEBUG(0, "Request button idx=%d pressed", request->idx);
            button_pressed(&changed, request->idx);
            break;
        }

        case QMS_REQUEST_BLUETOOTH_ON: {
            LOG_DEBUG(0, "Request bluetooth on");
            transition_state_bt_on(&changed);
            break;
        }

        case QMS_REQUEST_BLUETOOTH_OFF: {
            LOG_DEBUG(0, "Request bluetooth off");
            transition_state_bt_off(&changed, NULL);
            break;
        }

        case QMS_REQUEST_DEVICE_DISCONNECTED: {
            LOG_DEBUG(0, "Request device mac0=0x%08X mac1=0x%08X disconnected", request->mac.mac0, request->mac.mac1);
            int idx = mac_to_idx(request->mac.mac0, request->mac.mac1);
            if (idx < 0) {
                LOG_ERROR("mac0=0x%08X mac1=0x%08X not found", request->mac.mac0, request->mac.mac1);
            } else {
                transition_state_disconnected(&changed, idx, false);
            }
            break;
        }

        case QMS_REQUEST_DEVICE_CONNECTED: {
            LOG_DEBUG(0, "Request device mac0=0x%08X mac1=0x%08X connected", request->mac.mac0, request->mac.mac1);
            int idx = mac_to_idx(request->mac.mac0, request->mac.mac1);
            if (idx < 0) {
                LOG_ERROR("mac0=0x%08X mac1=0x%08X not found", request->mac.mac0, request->mac.mac1);
            } else {
                transition_state_connected(&changed, idx);
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
 * Initialize the mutex. TODO.
 */
int qm_state_mutex_start(void) {
    int ret = sceKernelCreateLwMutex(&mutex, "vqmbt-qm_state-mutex", 0, 0, NULL);
    if (ret < 0) {
        LOG_ERROR("sceKernelCreateLwMutex returned error 0x%08X", ret);
        return VQMBT_ERROR_GENERAL_FAILURE;
    }
    return 0;
}

/**
 * Delete the mutex. TODO.
 */
void qm_state_mutex_stop(void) {
    // Delete mutex.
    int ret = sceKernelDeleteLwMutex(&mutex);
    LOG_DEBUG(0, "sceKernelDeleteLwMutex returned 0x%08X", ret);
}
