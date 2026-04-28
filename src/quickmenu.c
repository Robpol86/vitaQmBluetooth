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
#define ID_SECTION_TEXT PROJECT_NAME "SectionText"
#define ID_LOADING_TEXT PROJECT_NAME "LoadingText"

/**
 * Loads the plugin's quick menu items.
 */
void quickmenu_start() {
    // Add horizontal line separator.
    QuickMenuRebornSeparator(ID_SEPARATOR, SCE_SEPARATOR_HEIGHT);

    // Add section heading text.
    QuickMenuRebornRegisterWidget(ID_SECTION_TEXT, NULL, text);
    QuickMenuRebornSetWidgetSize(ID_SECTION_TEXT, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_SECTION_TEXT, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_SECTION_TEXT, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_SECTION_TEXT, "Bluetooth Devices");

    // TODO add Bluetooth icon for section header.

    // TODO add ? or Info button that shows the version, build date, commit sha, github link. Like [Party].

    // Add placeholder "Loading" text.
    QuickMenuRebornRegisterWidget(ID_LOADING_TEXT, NULL, text);
    QuickMenuRebornSetWidgetSize(ID_LOADING_TEXT, SCE_PLANE_WIDTH, 75, 0, 0);
    QuickMenuRebornSetWidgetPosition(ID_LOADING_TEXT, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetColor(ID_LOADING_TEXT, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(ID_LOADING_TEXT, "Loading...");

    // TODO style: alignment and spacing. Then PR.

    // TODO exports.yml versioning? Project versioning? Maybe "Bluetooth Devices v1.0.2604261359" for debug and just v1.0 for
    // release? ACTUALLY keep version 1.0. Add build commit sha via cmake and expose in UI. Or use version x.x.x like
    // QuickMenuReborn does, but also keep sha.

    // TODO version.c.in with PROJECT_NAME exportd by CMake, as well as VERSION_MAJOR and VERSON_MINOR. Also use
    // exports.yml.in.

    // TODO PR and then convert to C++ in the next branch.
}

/**
 * Unloads the plugin's quick menu items.
 */
void quickmenu_stop() {
    QuickMenuRebornUnregisterWidget(ID_LOADING_TEXT);
    QuickMenuRebornUnregisterWidget(ID_SECTION_TEXT);
    QuickMenuRebornRemoveSeparator(ID_SEPARATOR);
}
