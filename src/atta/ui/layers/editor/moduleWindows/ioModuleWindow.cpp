//--------------------------------------------------
// Atta UI Module
// ioModuleWindow.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "stb_image.h"
#include <atta/ui/layers/editor/moduleWindows/ioModuleWindow.h>

namespace atta::ui {

IOModuleWindow::IOModuleWindow() { setName("IO Module"); }

void IOModuleWindow::renderImpl() {
    if (ImGui::BeginTabBar("##IOModuleWindowTab", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Serial##IOModuleWindowSerial")) {
            serialTabItem();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Camera##IOModuleWindowCamera")) {
            cameraTabItem();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Bluetooth##IOModuleWindowBluetooth")) {
            bluetoothTabItem();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    bluetoothViews();
    if (_bluetooth)
        _bluetooth->update();
}

} // namespace atta::ui

#include <atta/ui/layers/editor/moduleWindows/ioModuleWindowBluetooth.cpp>
#include <atta/ui/layers/editor/moduleWindows/ioModuleWindowCamera.cpp>
#include <atta/ui/layers/editor/moduleWindows/ioModuleWindowSerial.cpp>
