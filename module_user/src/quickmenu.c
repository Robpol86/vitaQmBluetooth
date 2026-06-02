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

#include "quickmenu.h"

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr/thread.h>
#include <quickmenureborn/c_types.h>
#include <quickmenureborn/qm_reborn.h>

#include "kmod_event.h"
#include "log.h"
#include "vqmbt.h"

#define BUTTON_LABEL_MAX (sizeof(((VqmbtDeviceInfo*)0)->name) + 16)

static VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES];  // TODO locking/semaphore?

// Widget IDs (prefixed because they must be unique across all plugins).
#define ID_SEPARATOR MODULE_NAME "Separator"
#define ID_PLANE_ROOT MODULE_NAME "PlaneRoot"
#define ID_PLANE_MESSAGE MODULE_NAME "PlaneMessage"
#define ID_PLANE_BUTTONS MODULE_NAME "PlaneButtons"
#define ID_SECTION_TEXT MODULE_NAME "SectionText"
static const char* const ID_BUTTONS[VQMBT_MAX_DEVICES] = {
    MODULE_NAME "Button0", MODULE_NAME "Button1", MODULE_NAME "Button2", MODULE_NAME "Button3",
    MODULE_NAME "Button4", MODULE_NAME "Button5", MODULE_NAME "Button6", MODULE_NAME "Button7",
};
_Static_assert(sizeof(ID_BUTTONS) / sizeof(ID_BUTTONS[0]) == VQMBT_MAX_DEVICES,
               "ID_BUTTONS size must match VQMBT_MAX_DEVICES");

/**
 * Connect or disconnect the device associated with the button.
 *
 * Called when the user taps on the button.
 *
 * TODO:
 * - Relabel button "Connecting <name>...".
 * - when user taps a button disable all buttons and wait for callback.
 */
BUTTON_HANDLER(quickmenu_on_press) {
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
ONLOAD_HANDLER(quickmenu_on_load) {
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
        const char* id = ID_BUTTONS[idx];
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
void quickmenu_on_unload(const char* id) {
    (void)id;

    LOG_DEBUG(0, "Quick menu closed.");

    // Reset button labels.
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        char label[BUTTON_LABEL_MAX];
        sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        QuickMenuRebornSetWidgetLabel(id, label);
    }
}

/**
 * TODO.
 *
 * TODO:
 * - callback: relabel button with new state. Surface error in button as close/reopen resets labels
 * - button_reset() button_disable() button_enable() functions
 * - TODO TODO TODO use a thread to dim/undim and size/resize widgets every 10s, can these be animated (e.g. fading)?
 * - long bt names shrink text not dissappear it
 */
static void create_widgets(void) {
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

    // Add device slot buttons.
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornRegisterWidget(id, ID_PLANE_ROOT, button);
        QuickMenuRebornSetWidgetSize(id, 600, 75, 0, 0);
        QuickMenuRebornSetWidgetPosition(id, 20, 243 - (idx * 80), 0, 0);
        QuickMenuRebornSetWidgetColor(id, 1, 1, 1, 1);
        char label[BUTTON_LABEL_MAX];
        sceClibSnprintf(label, sizeof(label), "Slot %d: no device", idx + 1);
        QuickMenuRebornSetWidgetLabel(id, label);
        QuickMenuRebornRegisterEventHanlder(id, QMR_BUTTON_RELEASE_ID, quickmenu_on_press, (void*)(intptr_t)idx);
    }

    // Register handlers.
    const char* last = ID_BUTTONS[VQMBT_MAX_DEVICES - 1];
    QuickMenuRebornAssignOnLoadHandler(quickmenu_on_load, last);
    QuickMenuRebornAssignOnDeleteHandler(quickmenu_on_unload, last);
}

/**
 * TODO
 */
static void remove_widgets(void) {
    for (int idx = 0; idx < VQMBT_MAX_DEVICES; idx++) {
        const char* id = ID_BUTTONS[idx];
        QuickMenuRebornUnregisterWidget(id);
    }
    QuickMenuRebornUnregisterWidget(ID_SECTION_TEXT);
    QuickMenuRebornUnregisterWidget(ID_PLANE_ROOT);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);
}

/**
 * TODO
 *
 * TODO:
 * - Develop Dockerfile that will build my QMR fork
 * - Update redist here to use newly built fork, confirm on vita
 * - Attempt to patch in show/hide/enable/disable APIs
 * - Test new APIs in dim_thread().
 *      - Hide/show the root plane, confirm buttons are unselectable whilst hidden, and no scrollbars
 *      - Enable/disable first button. Should be selectable as-per big BT button at the top, but tap/X/O no-ops
 */
static int dim_thread(SceSize args, void* argp) {
    (void)args;
    (void)argp;

    int state = 0;
    char label[BUTTON_LABEL_MAX];
    const char* first_button_id = ID_BUTTONS[0];

    while (false) {  // TODO enable to test
        state += 1;
        sceClibSnprintf(label, sizeof(label), "Switch state: %d", state);
        QuickMenuRebornSetWidgetLabel(first_button_id, label);
        switch (state) {
            case 1:
                // Dim button 50%
                QuickMenuRebornSetWidgetColor(first_button_id, 0.5F, 0.5F, 0.5F, 1.0F);
                break;
            case 2:
                // Undim button
                QuickMenuRebornSetWidgetColor(first_button_id, 1.0F, 1.0F, 1.0F, 1.0F);
                break;
            case 3:
                // Hide button
                // QuickMenuRebornUnregisterWidget(first_button_id);  // TODO Works but probably wrong approach
                QuickMenuRebornSetWidgetColor(first_button_id, 1.0F, 1.0F, 1.0F, 0.0F);
                break;
            case 4:
                // Unhide button
                // QuickMenuRebornRegisterWidget(first_button_id, ID_PLANE_ROOT, button);  // TODO COREDUMP
                QuickMenuRebornSetWidgetColor(first_button_id, 1.0F, 1.0F, 1.0F, 1.0F);
                break;
            case 5:
                // Shrink root plane
                QuickMenuRebornSetWidgetSize(ID_PLANE_ROOT, SCE_PLANE_WIDTH, 350.0F, 0, 0);
                break;
            case 6:
                // Unshrink root plane
                QuickMenuRebornSetWidgetSize(ID_PLANE_ROOT, SCE_PLANE_WIDTH, 700.0F, 0, 0);
                break;
            // case 7:
            //     // Swap root plane with message plane
            //     // TODO
            //     break;
            // case 8:
            //     // Swap back to root plane
            //     // TODO
            //     break;
            default:
                state = 0;
                break;
        }
        sceKernelDelayThread(5 * 1000000);
    }

    return 0;
}

static void dim_thread_start(void) {
    int ret = sceKernelCreateThread("vqmbt-kmod_event-event_thread", dim_thread, 0x96, 0x1000, 0,
                                    SCE_KERNEL_THREAD_CPU_AFFINITY_MASK_DEFAULT, NULL);
    if (ret < 0) {
        LOG_ERROR("sceKernelCreateThread returned error 0x%08X", ret);
        return;
    }
    sceKernelStartThread(ret, 0, NULL);
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
 */
void quickmenu_start(void) {
    // TODO
    kmod_event_start();  // TODO move into quickmenu_on_load

    // TODO
    create_widgets();

    // TODO remove
    dim_thread_start();
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop(void) {
    // TODO
    remove_widgets();

    // TODO
    kmod_event_stop();  // TODO move into quickmenu_on_unload
}
