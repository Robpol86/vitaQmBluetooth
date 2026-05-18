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
#define ID_SEPARATOR PROJECT_NAME "Separator"
#define ID_PLANE_ROOT PROJECT_NAME "PlaneRoot"
#define ID_SECTION_TEXT PROJECT_NAME "SectionText"
#define ID_LOADING_TEXT PROJECT_NAME "LoadingText"
#define ID_BUTTON PROJECT_NAME "Button"

/**
 * Called when the user taps on the button. Emits a log message.
 */
BUTTON_HANDLER(on_press) {
    (void)id;
    (void)hash;
    (void)eventId;
    (void)userDat;

    LOG_DEBUG(0, "Calling kernel functions.");

    VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES];
    VqmbtDeviceInfo* dev;
    int count = kvqmbtGetPairedDevices(devices, VQMBT_MAX_DEVICES);
    if (count > 0) {
        LOG_DEBUG(0, "count=%d", count);
        int conn_disconn_idx = 0;
        // Log.
        for (int idx = 0; idx < count; idx++) {
            dev = &devices[idx];
            LOG_DEBUG(0, "idx=%d name=\"%s\" mac0=0x%08X mac1=0x%08X", idx, dev->name, dev->mac0, dev->mac1);
            if (sceClibStrncmp(dev->name, "APP Scuffed", 11) == 0) {
                LOG_DEBUG(0, "Set conn_disconn_dev to %d for device \"%s\"", idx, dev->name);
                conn_disconn_idx = idx;
            }
        }
        // Connect/disconnect.
        dev = &devices[conn_disconn_idx];
        if (kvqmbtIsConnected(dev->mac0, dev->mac1)) {
            LOG_DEBUG(0, "Disconnecting \"%s\"", dev->name);
            kvqmbtDisconnectDevice(dev->mac0, dev->mac1);
        } else {
            LOG_DEBUG(0, "Connecting \"%s\"", dev->name);
            kvqmbtConnectDevice(dev->mac0, dev->mac1);
        }
    } else {
        LOG_ERROR("kvqmbtGetPairedDevices returned error: 0x%08X", count);
    }

    LOG_DEBUG(0, "Done calling kernel functions.");
}

/**
 * Called when the quick menu is opened by the user.
 */
ONLOAD_HANDLER(on_load) {
    (void)id;

    LOG_DEBUG(0, "Quick menu opened.");
}

/**
 * Called when the quick menu is closed by the user.
 */
void on_unload(const char* id) {
    (void)id;

    LOG_DEBUG(0, "Quick menu closed.");
}

/**
 * Loads the plugin's quick menu items.
 *
 * TODOs:
 * - Add function to calculate position from top left instead of center.
 * - Pixel perfect alignment.
 * - If kernel plugin isn't loaded notify user.
 */
void quickmenu_start(void) {
    // Add horizontal line separator.
    QuickMenuRebornSeparator(ID_SEPARATOR, SCE_SEPARATOR_HEIGHT);

    // Add the root plane that holds all other items.
    QuickMenuRebornRegisterWidget(ID_PLANE_ROOT, NULL, plane);
    QuickMenuRebornSetWidgetSize(ID_PLANE_ROOT, SCE_PLANE_WIDTH, 200, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_PLANE_ROOT, 1, 1, 1, 0);

    // Add section heading text.
    QuickMenuRebornRegisterWidget(ID_SECTION_TEXT, ID_PLANE_ROOT, text);
    QuickMenuRebornSetWidgetSize(ID_SECTION_TEXT, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_SECTION_TEXT, -206, 62, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_SECTION_TEXT, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_SECTION_TEXT, "Bluetooth Devices");

    // Add placeholder "Loading" text.
    QuickMenuRebornRegisterWidget(ID_LOADING_TEXT, ID_PLANE_ROOT, text);
    QuickMenuRebornSetWidgetSize(ID_LOADING_TEXT, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_LOADING_TEXT, -220, -3, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_LOADING_TEXT, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_LOADING_TEXT, "Loading...");

    // Add button to test emitting logs.
    QuickMenuRebornRegisterWidget(ID_BUTTON, ID_PLANE_ROOT, button);
    QuickMenuRebornSetWidgetSize(ID_BUTTON, 200, 75, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_BUTTON, -220, -83, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_BUTTON, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_BUTTON, "Emit Log");
    QuickMenuRebornRegisterEventHanlder(ID_BUTTON, QMR_BUTTON_RELEASE_ID, on_press, NULL);

    // Register handlers.
    QuickMenuRebornAssignOnLoadHandler(on_load, ID_PLANE_ROOT);
    QuickMenuRebornAssignOnDeleteHandler(on_unload, ID_PLANE_ROOT);
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop(void) {
    QuickMenuRebornUnregisterWidget(ID_BUTTON);
    QuickMenuRebornUnregisterWidget(ID_LOADING_TEXT);
    QuickMenuRebornUnregisterWidget(ID_SECTION_TEXT);
    QuickMenuRebornUnregisterWidget(ID_PLANE_ROOT);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);
}
