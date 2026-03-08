//--------------------------------------------------
// Simple Script
// projectScript.cpp
// Date: 2021-11-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"

void Project::onLoad() {
    // Executed when the script is loaded by the atta. The script need to be reloaded every time it is recompiled
    LOG_DEBUG("Project", "onLoad");
}

void Project::onUnload() {
    // Execute just before the script is unloaded. Usually it is unloaded to load the new compiled script.
    LOG_DEBUG("Project", "onUnload");
}

void Project::onStart() {
    // Executed when the simulation start
    LOG_DEBUG("Project", "onStart");
}

void Project::onStop() {
    // Executed when the simulation is stopped
    LOG_DEBUG("Project", "onStop");
}

void Project::onContinue() {
    // Executed when the simulation continue after pause
    LOG_DEBUG("Project", "onContinue");
}

void Project::onPause() {
    // Executed when the simulation is paused
    LOG_DEBUG("Project", "onPause");
}

void Project::onUpdateBefore(float delta) {
    // Executed before all entity scripts
    // LOG_DEBUG("Project", "onUpdateBef");
}

void Project::onUpdateAfter(float delta) {
    // Executed after all entity scripts
}

void Project::onUIRender() {
    // UI Render instructions
    // It is used to create a user interface and interface with the simulation.
    // Possible uses are window creation, expose variables to the user, show current simulation status
}
