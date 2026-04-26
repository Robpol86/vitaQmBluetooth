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
 * Interface for the Quick Menu on the PS Vita.
 */

#include <psp2/kernel/modulemgr.h>
#include <quickmenureborn/qm_reborn.h>

// Widget IDs (prefixed because they must be unique across all plugins).
#define PROJECT_NAME "vitaQmBluetooth"
#define SEPARATOR_ID PROJECT_NAME "Separator"
#define SECTION_TEXT_ID PROJECT_NAME "SectionText"

/**
 * Loads the plugin's quick menu items.
 */
void quickmenu_start() {
    QuickMenuRebornSeparator(SEPARATOR_ID, SCE_SEPARATOR_HEIGHT);

    QuickMenuRebornRegisterWidget(SECTION_TEXT_ID, NULL, text);
    QuickMenuRebornSetWidgetSize(SECTION_TEXT_ID, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetColor(SECTION_TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetPosition(SECTION_TEXT_ID, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(SECTION_TEXT_ID, "Bluetooth Devices 3");  // TODO
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop() {
    QuickMenuRebornUnregisterWidget(SECTION_TEXT_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID);
}
