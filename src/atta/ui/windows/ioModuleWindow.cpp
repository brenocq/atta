//--------------------------------------------------
// Atta UI Module
// ioModuleWindow.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "stb_image.h"
#include <atta/ui/windows/ioModuleWindow.h>

namespace atta::ui {

IOModuleWindow::IOModuleWindow() {
    initialize("IO Module");
    _initialSize = vec2(350.0f, 500.0f);
}

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

#include <atta/ui/windows/ioModuleWindowBluetooth.cpp>
#include <atta/ui/windows/ioModuleWindowCamera.cpp>
#include <atta/ui/windows/ioModuleWindowSerial.cpp>
