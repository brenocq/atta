//--------------------------------------------------
// Atta UI Module
// ioModuleWindow.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "stb_image.h"
#include <atta/ui/editor/moduleWindows/ioModuleWindow.h>

namespace atta::ui {

IOModuleWindow::IOModuleWindow() {
    setName("IO Module");
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

#include <atta/ui/editor/moduleWindows/ioModuleWindowBluetooth.cpp>
#include <atta/ui/editor/moduleWindows/ioModuleWindowCamera.cpp>
#include <atta/ui/editor/moduleWindows/ioModuleWindowSerial.cpp>
