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
#define ID_SEPARATOR PROJECT_NAME "Separator"
#define ID_PLANE_ROOT PROJECT_NAME "PlaneRoot"
#define ID_SECTION_TEXT PROJECT_NAME "SectionText"
#define ID_LOADING_TEXT PROJECT_NAME "LoadingText"

/**
 * Loads the plugin's quick menu items.
 *
 * TODOs:
 * - Add function to calculate position from top left instead of center.
 * - Pixel perfect alignment.
 */
void quickmenu_start() {
    // Add horizontal line separator.
    QuickMenuRebornSeparator(ID_SEPARATOR, SCE_SEPARATOR_HEIGHT);

    // Add the root plane that holds all other items.
    QuickMenuRebornRegisterWidget(ID_PLANE_ROOT, NULL, plane);
    QuickMenuRebornSetWidgetSize(ID_PLANE_ROOT, SCE_PLANE_WIDTH, 125, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_PLANE_ROOT, 1, 1, 1, 0);

    // Add section heading text.
    QuickMenuRebornRegisterWidget(ID_SECTION_TEXT, ID_PLANE_ROOT, text);
    QuickMenuRebornSetWidgetSize(ID_SECTION_TEXT, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_SECTION_TEXT, -206, 25, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_SECTION_TEXT, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_SECTION_TEXT, "Bluetooth Devices");

    // Add placeholder "Loading" text.
    QuickMenuRebornRegisterWidget(ID_LOADING_TEXT, ID_PLANE_ROOT, text);
    QuickMenuRebornSetWidgetSize(ID_LOADING_TEXT, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_LOADING_TEXT, -220, -40, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_LOADING_TEXT, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_LOADING_TEXT, "Loading...");

    // TODO style: alignment and spacing. Then PR.
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop() {
    QuickMenuRebornUnregisterWidget(ID_LOADING_TEXT);
    QuickMenuRebornUnregisterWidget(ID_SECTION_TEXT);
    QuickMenuRebornUnregisterWidget(ID_PLANE_ROOT);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);
}
