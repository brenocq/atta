//--------------------------------------------------
// Atta UI System
// ioSystemWindow.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindow.h>
#include "stb_image.h"

namespace atta::ui
{
    IOSystemWindow::IOSystemWindow()
    {
        setName("IO System");
    }

    void IOSystemWindow::renderImpl()
    {
        if(ImGui::BeginTabBar("##IOSystemWindowTab", ImGuiTabBarFlags_None))
        {
            if(ImGui::BeginTabItem("Serial##IOSystemWindowSerial"))
            {
                serialTabItem();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Camera##IOSystemWindowCamera"))
            {
                cameraTabItem();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Bluetooth##IOSystemWindowBluetooth"))
            {
                bluetoothTabItem();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
}

#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindowBluetooth.cpp>
#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindowCamera.cpp>
#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindowSerial.cpp>
