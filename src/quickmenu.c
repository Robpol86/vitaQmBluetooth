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
#define LOADING_TEXT_ID PROJECT_NAME "LoadingText"

/**
 * Loads the plugin's quick menu items.
 */
void quickmenu_start() {
    // Add horizontal line separator.
    QuickMenuRebornSeparator(SEPARATOR_ID, SCE_SEPARATOR_HEIGHT);

    // Add section heading text.
    QuickMenuRebornRegisterWidget(SECTION_TEXT_ID, NULL, text);
    QuickMenuRebornSetWidgetSize(SECTION_TEXT_ID, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetPosition(SECTION_TEXT_ID, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetColor(SECTION_TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(SECTION_TEXT_ID, "Bluetooth Devices");

    // TODO add Bluetooth icon for section header.

    // TODO add ? or Info button that shows the version, build date, commit sha, github link. Like [Party].

    // Add placeholder "Loading" text.
    QuickMenuRebornRegisterWidget(LOADING_TEXT_ID, NULL, text);
    QuickMenuRebornSetWidgetSize(LOADING_TEXT_ID, SCE_PLANE_WIDTH, 75, 0, 0);
    QuickMenuRebornSetWidgetPosition(LOADING_TEXT_ID, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetColor(LOADING_TEXT_ID, 1, 1, 1, 1);
    QuickMenuRebornSetWidgetLabel(LOADING_TEXT_ID, "Loading...");

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
    QuickMenuRebornUnregisterWidget(LOADING_TEXT_ID);
    QuickMenuRebornUnregisterWidget(SECTION_TEXT_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID);
}
