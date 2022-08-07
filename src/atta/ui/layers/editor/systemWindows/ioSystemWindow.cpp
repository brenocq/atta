//--------------------------------------------------
// Atta UI Module
// ioSystemWindow.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/systemWindows/ioSystemWindow.h>
#include "stb_image.h"

namespace atta::ui
{
    IOSystemWindow::IOSystemWindow()
    {
        setName("IO Module");
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

        bluetoothViews();
        if(_bluetooth)
            _bluetooth->update();
    }
}

#include <atta/ui/layers/editor/systemWindows/ioSystemWindowBluetooth.cpp>
#include <atta/ui/layers/editor/systemWindows/ioSystemWindowCamera.cpp>
#include <atta/ui/layers/editor/systemWindows/ioSystemWindowSerial.cpp>
